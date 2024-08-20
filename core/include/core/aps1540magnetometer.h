#ifndef WIGGLEWATCHER_APS1540MAGNETOMETER_H
#define WIGGLEWATCHER_APS1540MAGNETOMETER_H

#include <core/magnetometer.h>

class WIGGLEWATCHER_CORE_EXPORT Aps1540Magnetometer : public Magnetometer {
Q_OBJECT

public:
    explicit Aps1540Magnetometer(QIODevice *io, QObject *parent = nullptr);

    ~Aps1540Magnetometer() override = default;

    [[nodiscard]] auto type() const noexcept -> wigglewatcher::SensorKind override { return wigglewatcher::SensorKind::APS1540; }

    static auto stringToData(const QString &string) -> VectorMagnetometerData;

protected:
    auto handleReadyRead(QIODevice *device) -> void override;
};
#endif //WIGGLEWATCHER_APS1540MAGNETOMETER_H
