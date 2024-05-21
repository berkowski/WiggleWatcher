#include <core/aps1540magnetometer.h>
#include <core/iofactory.h>
#include <core/textfilesink.h>

#include "state.h"
#include "mainwindow.h"

#include <QtWidgets/qapplication.h>
#include <QtCore/qdir.h>
#include <QtCore/qthread.h>

#include <chrono>

int main(int argc, char *argv[])
{
    auto app = QApplication{argc, argv};

    // read settings

    // setup gui
    auto gui = MainWindow{};

    // initialize state
    auto state = StateObject{&app, &gui};

    // setup IO threads
    auto raw_logger = TextFileSink{QDir::tempPath(), QStringLiteral("raw"), QStringLiteral(".txt")};
    auto io = IOFactory::from_string("udp://127.0.0.1:70001:70000");
    if (!io) {
        qFatal() << "Unable to open udp socket";
    }
    auto aps1540 = Aps1540Magnetometer{io};

    auto io_threads = QList<QThread*>{};
    {
        auto io_thread = QThread::create([&]() {

            raw_logger.setRolloverInterval(std::chrono::seconds(10));
            QObject::connect(&app, &QApplication::aboutToQuit, &raw_logger, &TextFileSink::flush);
            QObject::connect(&state, &StateObject::stateChanged, [&](const auto& s) {
                if(s.log_directory != raw_logger.dir().absolutePath()) {
                    qInfo() << "Changing logging directory to " << s.log_directory;
                    raw_logger.setDir(QDir{s.log_directory});
                }

                if(s.recording != raw_logger.isActive()) {
                    if (s.recording) {
                        raw_logger.start();
                    }
                    else {
                        raw_logger.stop();
                    }
                }
            });

            QObject::connect(&aps1540, &Aps1540Magnetometer::bytesRead, &raw_logger, &TextFileSink::write);
	        QObject::connect(&aps1540, &Magnetometer::valueChanged, [&](const VectorMagnetometerData& data) {
	        gui.addVectorMagnetometerData("aps1540", data);
	    });

            // QObject::connect(&aps1540, &Aps1540Magnetometer::bytesRead, [&](const auto &bytes) {
            //     qInfo() << QString::fromUtf8(bytes);
            // });
        });
        io_thread->start();
        io_threads.push_back(io_thread);
    }
    // make connections
    QObject::connect(&state, &StateObject::stateChanged, &gui, &MainWindow::updateState);
    QObject::connect(&gui,
                     &MainWindow::setRecordingTriggered, &state, &StateObject::toggleRecordingEnabled);
    QObject::connect(&gui, &MainWindow::logDirectoryChanged, &state, &StateObject::setLogDirectory);
    
    // propagate initial state to widgets
    state.stateChanged(state.currentState());


    gui.show();

    return QApplication::exec();
}
