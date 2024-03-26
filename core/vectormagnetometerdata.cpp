#include <core/vectormagnetometerdata.h>

auto operator<<(QDebug dbg, const VectorMagnetometerData &data) -> QDebug
{
    dbg.nospace() << "VectorMagnetometer{timestamp: "
                  << data.timestamp.toString(Qt::DateFormat::ISODateWithMs) << ", x: " << data.x
                  << ", y: " << data.y << ", z: " << data.z << ", temp: " << data.temperature
                  << "}";
    return dbg;
}
