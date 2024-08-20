#ifndef WIGGLEWATCHER_MAGNETOMETERLOGGER_H
#define WIGGLEWATCHER_MAGNETOMETERLOGGER_H

#include <core/global.h>
#include <core/sensorlogger.h>

class Magnetometer;

class WIGGLEWATCHER_CORE_EXPORT MagnetometerLogger: public SensorLogger
{
    Q_OBJECT
public:
    explicit WIGGLEWATCHER_CORE_EXPORT MagnetometerLogger(Magnetometer* magnetometer, QObject* parent=nullptr);
};

#endif //WIGGLEWATCHER_MAGNETOMETERLOGGER_H
