#include "core/magnetometerlogger.h"
#include "core/sensorlogger.h"
#include "core/magnetometer.h"
#include "core/textfilesink.h"
#include "core/vectormagnetometerdata.h"

MagnetometerLogger::MagnetometerLogger(Magnetometer *magnetometer, QObject *parent)
    : SensorLogger(magnetometer, parent)
{

    // emit on new data
    const auto name = magnetometer->name();
    const auto kind = magnetometer->type();
    QObject::connect(magnetometer, &Magnetometer::valueChanged, [=](const auto& value) {
        this->valueChanged(name, kind, QVariant::fromValue(value));
    });

    // log new data in dsl format
    QObject::connect(magnetometer, &Magnetometer::valueChanged, [=](const auto& value) {
        sink->write(value.toDslFormat().toUtf8());
    });

}
