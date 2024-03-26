#include <core/vectormagnetometerdata.h>

auto VectorMagnetometerData::toDslFormat() const noexcept -> QString
{
    auto string = QString{};
    auto stream = QTextStream{&string};
    stream.setRealNumberNotation(QTextStream::FixedNotation);
    stream.setNumberFlags(QTextStream::ForceSign);
    stream.setFieldWidth(12);
    stream.setRealNumberPrecision(2);
    stream << timestamp.toString(Qt::ISODateWithMs) << x << y << z << temperature
           << QStringLiteral("\n");

    return string;
}

auto operator<<(QDebug dbg, const VectorMagnetometerData &data) -> QDebug
{
    dbg.nospace() << "VectorMagnetometer{timestamp: "
                  << data.timestamp.toString(Qt::DateFormat::ISODateWithMs) << ", x: " << data.x
                  << ", y: " << data.y << ", z: " << data.z << ", temp: " << data.temperature
                  << "}";
    return dbg;
}
