#include "state.h"
#include "mainwindow.h"

#include <core/aps1540magnetometer.h>
#include <core/textfilesink.h>
#include <core/settings.h>
#include <core/sensorfactory.h>
#include <core/magnetometerlogger.h>

#include <libqio/iofactory.h>

#include <QApplication>
#include <QDir>
#include <QThread>
#include <QStandardPaths>
#include <QList>
#include <QPair>
#include <QMessageBox>
#include <QVariant>
#include <QScopedPointer>
#include <QStatusBar>

int main(int argc, char *argv[])
{
    auto app = QApplication{argc, argv};

    const auto search_paths = [](){
        const auto CONFIG_PAIRS = QList<QPair<QDir, QString>> {
            { QApplication::applicationDirPath(), QStringLiteral("config.toml")},
            { QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation), QStringLiteral("config.toml") },
            { QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation), QStringLiteral("wigglewatcher.toml")},
            { QStandardPaths::writableLocation(QStandardPaths::HomeLocation), QStringLiteral("wigglewatcher.toml")}
        };
        auto paths = QStringList{};
        std::for_each(std::begin(CONFIG_PAIRS), std::end(CONFIG_PAIRS),[&](const auto& pair) {
           paths.push_back(pair.first.absoluteFilePath(pair.second));
        });
        return paths;
    }();

    const auto config_file = std::find_if(std::begin(search_paths), std::end(search_paths), [&](const auto& path) {
        return QFileInfo::exists(path);
    });

    // can't find config, show an error and quit
    if (config_file == std::end(search_paths)) {
        auto error_message = QString{"Could not find config file in the following locations:\n\n"};
        std::for_each(std::begin(search_paths), std::end(search_paths), [&](const auto &path) {
            error_message += QStringLiteral("%1\n").arg(path);
        });

        QMessageBox::critical(nullptr, QStringLiteral("Unable to load configuration"), error_message);
        return 0;
    }

    // load settings from file
    const auto settings = Settings::fromFile(*config_file);
    if (!settings.isValid()) {
        auto error_message = QString{"Unable to load config from %1"}.arg(*config_file);
        QMessageBox::critical(nullptr, QStringLiteral("Unable to load configuration"), error_message);
        return 0;
    }

    // setup gui
    auto gui = MainWindow{};
    gui.setWindowTitle(QStringLiteral("WiggleWatcher v%1").arg(wigglewatcher::WIGGLEWATCHER_VERSION));

    // initialize state
    auto state = StateObject{&app, &gui};
    state.setLogDirectory(settings.dir());

    // spin up IO threads;
    for(const auto& config: settings.sensors()) {
        auto sensor = SensorFactory::create(config->kind, config->name, config->connection, config->description);
        if(!sensor) {
            auto error_message = QString{"Unable to load config from %1\n\n%2"}.arg(*config_file, SensorFactory::last_error_string());
            QMessageBox::critical(nullptr, QStringLiteral("Unable to load configuration"), error_message);
            return 0;
        }

        switch (sensor->type()) {
        case wigglewatcher::SensorKind::APS1540:
        case wigglewatcher::SensorKind::HMR2300:
        {
            auto logger = new MagnetometerLogger(qobject_cast<Magnetometer*>(sensor.release()), &app);

            // Set file interval
            logger->setRolloverInterval(settings.interval());

            // connect state updates
            QObject::connect(&state, &StateObject::stateChanged, [&](const auto& s) {
                logger->setLogDirectory(s.log_directory);
                logger->setLoggingEnabled(s.recording);
            });

            // connect plot
            QObject::connect(logger, &MagnetometerLogger::valueChanged, qApp, [&](const auto& name, const auto& kind, const auto& data) {
                gui.addVectorMagnetometerData(name, qvariant_cast<VectorMagnetometerData>(data));
            }, Qt::QueuedConnection);

            QObject::connect(logger, &MagnetometerLogger::bytesWritten, &state, &StateObject::updateBytesWritten, Qt::QueuedConnection);
            QObject::connect(logger, &MagnetometerLogger::bytesRead, &state, &StateObject::updateBytesRead, Qt::QueuedConnection);
            QObject::connect(logger, &MagnetometerLogger::valueChanged, &state, [&](const auto& name, const auto& kind, const auto& data) {
                state.updateSampleCount(1);
            }, Qt::QueuedConnection);

            break;
        }
        default:
            Q_UNREACHABLE();
        }
    }

    // make connections
    QObject::connect(&state, &StateObject::stateChanged, &gui, &MainWindow::updateState);
    QObject::connect(&gui,
                     &MainWindow::setRecordingTriggered, &state, &StateObject::toggleRecordingEnabled);
    QObject::connect(&gui, &MainWindow::logDirectoryChanged, &state, &StateObject::setLogDirectory);

    // propagate initial state to widgets
    state.stateChanged(state.currentState());

    // Display config file in status bar
    gui.statusBar()->showMessage(QStringLiteral("config file: %1").arg(*config_file));
    gui.show();

    return QApplication::exec();
}
