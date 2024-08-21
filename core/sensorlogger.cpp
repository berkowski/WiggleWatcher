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

    QObject::connect(sink, &TextFileSink::bytesWritten, this, &SensorLogger::bytesWritten, Qt::QueuedConnection);
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

void SensorLogger::setRolloverInterval(const std::chrono::milliseconds& duration) noexcept
{
    if (duration == sink->rolloverIntervalAsDuration()) {
        return;
    }
    
    const auto active = sink->isActive();
    
    if (active) {
        sink->stop();
    }
    
    sink->setRolloverInterval(duration);
    
    if (active) {
        sink->start();
    }
};
