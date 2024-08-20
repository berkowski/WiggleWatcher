#ifndef WIGGLEWATCHER_MAGNETOMETER_H
#define WIGGLEWATCHER_MAGNETOMETER_H

#include <core/global.h>
#include <core/sensorbase.h>
#include <core/vectormagnetometerdata.h>

class WIGGLEWATCHER_CORE_EXPORT Magnetometer : public SensorBase
{
    Q_OBJECT
public:
    Q_SIGNAL void valueChanged(const VectorMagnetometerData &);

    explicit Magnetometer(QIODevice *io, QObject *parent = nullptr);
    ~Magnetometer() override = default;
};

#endif //WIGGLEWATCHER_MAGNETOMETER_H
