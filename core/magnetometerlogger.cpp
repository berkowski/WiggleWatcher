#include "core/magnetometerlogger.h"
#include "core/sensorlogger.h"
#include "core/magnetometer.h"
#include "core/textfilesink.h"
#include "core/vectormagnetometerdata.h"

MagnetometerLogger::MagnetometerLogger(Magnetometer *magnetometer, QObject *parent)
    : SensorLogger(magnetometer, parent)
{

    // emit on new data
    QObject::connect(magnetometer, &Magnetometer::valueChanged, [&](const auto& value) {
        emit valueChanged(magnetometer->name(), magnetometer->type(), QVariant::fromValue(value));
    });

    // log new data in dsl format
    QObject::connect(magnetometer, &Magnetometer::valueChanged, [&](const auto& value) {
        sink->write(value.toDslFormat().toUtf8());
    });

}
