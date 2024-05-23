#ifndef MAGGUI_SENSORLOGGER_H
#define MAGGUI_SENSORLOGGER_H

#include <core/global.h>

#include <QObject>

class SensorBase;
class TextFileSink;
class QThread;

class MAGGUI_CORE_EXPORT SensorLogger: public QObject
{
    Q_OBJECT
public:
    ~SensorLogger() override;
    Q_SIGNAL void valueChanged(const QString& name, const maggui::SensorKind& kind, const QVariant& value);

    Q_SLOT void setLogDirectory(const QString& directory);
    Q_SLOT void setLoggingEnabled(bool enabled);

protected:
    explicit MAGGUI_CORE_EXPORT SensorLogger(SensorBase* base, QObject* parent=nullptr);
    QThread* thread;
    SensorBase* sensor;
    TextFileSink* sink;
};

#endif //MAGGUI_SENSORLOGGER_H
