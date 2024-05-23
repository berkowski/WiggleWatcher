#ifndef MAGGUI_MAGNETOMETERLOGGER_H
#define MAGGUI_MAGNETOMETERLOGGER_H

#include <core/global.h>
#include <core/sensorlogger.h>

class Magnetometer;

class MAGGUI_CORE_EXPORT MagnetometerLogger: public SensorLogger
{
    Q_OBJECT
public:
    explicit MAGGUI_CORE_EXPORT MagnetometerLogger(Magnetometer* magnetometer, QObject* parent=nullptr);
};

#endif //MAGGUI_MAGNETOMETERLOGGER_H
