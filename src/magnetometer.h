#ifndef MAGGUI_MAGNETOMETER_H
#define MAGGUI_MAGNETOMETER_H

#include "maggui-core_global.h"
#include "sensorbase.h"
#include "types/vectormagnetometerdata.h"

class MAGGUI_CORE_EXPORT Magnetometer : public SensorBase
{
    Q_OBJECT
public:
    Q_SIGNAL void valueChanged(const VectorMagnetometerData &);

    explicit Magnetometer(QIODevice *io, QObject *parent = nullptr);
    ~Magnetometer() override = default;
};

#endif //MAGGUI_MAGNETOMETER_H
