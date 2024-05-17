#include <core/aps1540magnetometer.h>
#include <core/iofactory.h>
#include <core/textfilesink.h>

#include "state.h"
#include "logcontrolwidget.h"

#include <QtWidgets/qapplication.h>
#include <QtCore/qdir.h>

#include <chrono>

int main(int argc, char *argv[])
{
    auto app = QApplication{argc, argv};

    // initialize state
    auto state = StateObject{};


    // setup gui
    auto gui = LogControlWidget{};
    
    // make connections
    QObject::connect(&state, &StateObject::stateChanged, &gui, &LogControlWidget::updateState);
    QObject::connect(&gui, &LogControlWidget::userSetRecordingEnabled, &state, &StateObject::setRecordingEnabled);
    QObject::connect(&gui, &LogControlWidget::userChangedLogDirectory, &state, &StateObject::setLogDirectory);
    
    // propagate initial state to widgets
    state.stateChanged(state.currentState());

    #if 0
    const auto log_root = QDir::tempPath();
    qInfo() << "Logging to: " << log_root;
    auto raw_logger = TextFileSink{log_root, QStringLiteral("raw"), QStringLiteral(".txt")};
    raw_logger.setRolloverInterval(std::chrono::seconds(10));

    auto io = IOFactory::from_string("udp://127.0.0.1:70001:70000");
    if (!io) {
        qFatal() << "Unable to open udp socket";
    }

    auto aps1540 = Aps1540Magnetometer{io};

    //    QObject::connect(&aps1540, &Aps1540Magnetometer::bytesRead, &raw_logger, &TextFileSink::write);
    QObject::connect(&aps1540, &Aps1540Magnetometer::valueChanged, [&](const auto &value) {
        raw_logger.write(value.toDslFormat().toUtf8());
    });

    QObject::connect(&app, &QCoreApplication::aboutToQuit, &raw_logger, &TextFileSink::flush);
    QObject::connect(&aps1540, &Aps1540Magnetometer::bytesRead, [&](const auto &bytes) {
        qInfo() << QString::fromUtf8(bytes);
    });
    #endif

    gui.show();

    return QApplication::exec();
}
