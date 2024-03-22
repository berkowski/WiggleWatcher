#ifndef MAGGUI_VECTORMAGNETOMETERDATA_H
#define MAGGUI_VECTORMAGNETOMETERDATA_H

#include "../maggui-core_global.h"

#include <QtCore/qdatetime.h>
#include <QtCore/qmetatype.h>

struct MAGGUI_CORE_EXPORT VectorMagnetometerData
{
    inline VectorMagnetometerData() noexcept;
    explicit inline VectorMagnetometerData(
        QDateTime datetime, double x, double y, double z, double temperature);
    [[nodiscard]] inline auto isNull() const noexcept -> bool;

    inline auto operator==(VectorMagnetometerData const &other) const -> bool;

    //! Timestamp of data
    QDateTime timestamp;
    //! instrument-frame X component of magnetic field in nano-tesla
    double x;
    //! instrument-frame Y component of magnetic field in nano-tesla
    double y;
    //! instrument-frame Z component of magnetic field in nano-tesla
    double z;
    //! instrument temperature in celcius
    double temperature;
};

Q_DECLARE_METATYPE(VectorMagnetometerData)

VectorMagnetometerData::VectorMagnetometerData() noexcept
    : VectorMagnetometerData(QDateTime{}, 0.0, 0.0, 0.0, 0.0)
{}

VectorMagnetometerData::VectorMagnetometerData(
    QDateTime datetime, double x_, double y_, double z_, double t_)
    : timestamp(std::move(datetime))
    , x(x_)
    , y(y_)
    , z(z_)
    , temperature(t_)
{}

auto VectorMagnetometerData::isNull() const noexcept -> bool
{
    return timestamp.isNull() && x == 0.0 && y == 0.0 && z == 0.0 && temperature == 0.0;
}

auto VectorMagnetometerData::operator==(const VectorMagnetometerData &other) const -> bool
{
    return !isNull() && !other.isNull() && timestamp == other.timestamp && x == other.x
           && y == other.y && z == other.z;
}

MAGGUI_CORE_EXPORT auto operator<<(QDebug dbg, const VectorMagnetometerData &data) -> QDebug;
#endif
