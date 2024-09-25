#ifndef WIGGLEWATCHER_SENSORLOGGER_H
#define WIGGLEWATCHER_SENSORLOGGER_H

#include <core/global.h>

#include <QObject>

class SensorBase;
class TextFileSink;
class QThread;

class WIGGLEWATCHER_CORE_EXPORT SensorLogger: public QObject
{
    Q_OBJECT
public:
    ~SensorLogger() override;
    Q_SIGNAL void valueChanged(const QString& name, const wigglewatcher::SensorKind& kind, const QVariant& value);
    Q_SIGNAL void bytesWritten(qint64);
    Q_SIGNAL void bytesRead(qint64);

    Q_SLOT void setLogDirectory(const QString& directory);
    Q_SLOT void setLoggingEnabled(bool enabled);

    Q_SLOT void setRolloverInterval(const std::chrono::milliseconds& duration) noexcept;
    auto name() const noexcept -> QString;

protected:
    explicit WIGGLEWATCHER_CORE_EXPORT SensorLogger(SensorBase* base, QObject* parent=nullptr);
    QThread* thread;
    SensorBase* sensor;
    TextFileSink* sink;
};

#endif //WIGGLEWATCHER_SENSORLOGGER_H
