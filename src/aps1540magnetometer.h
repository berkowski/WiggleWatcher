#ifndef MAGGUI_APS1540MAGNETOMETER_H
#define MAGGUI_APS1540MAGNETOMETER_H

#include "magnetometer.h"

class Aps1540Magnetometer : public Magnetometer
{
    Q_OBJECT

public:
    explicit Aps1540Magnetometer(QIODevice *io, QObject *parent = nullptr);
    ~Aps1540Magnetometer() override = default;

    static auto stringToData(const QString &string) -> VectorMagnetometerData;

protected:
    auto handleReadyRead(QIODevice *device) -> void override;
};

#endif //MAGGUI_APS1540MAGNETOMETER_H
