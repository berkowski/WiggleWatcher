#include "core/sensorlogger.h"
#include "core/sensorbase.h"
#include "core/textfilesink.h"
#include <QThread>

SensorLogger::SensorLogger(SensorBase *base, QObject *parent) : QObject(parent)
, thread(new QThread(this))
, sensor(base)
{
    const auto prefix = base->name().replace(' ', "_");
    const auto suffix = QStringLiteral(".txt");
    sink = new TextFileSink{{}, prefix, suffix};

    sink->moveToThread(thread);
    sensor->moveToThread(thread);
    thread->start();
}
SensorLogger::~SensorLogger() {
    // Allow thread to exit cleanly
    thread->quit();
    while (thread->isRunning()) {}
}

void SensorLogger::setLogDirectory(const QString &directory)
{
    sink->setDir(QDir{directory});
}

void SensorLogger::setLoggingEnabled(bool enabled) {
    if (enabled) {
        sink->start();
    }
    else {
        sink->stop();
    }
}
