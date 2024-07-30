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

    sink->setHeader(QStringLiteral("# name: %1\n# kind: %2\n# description: %3\n# Time  x(nT) y(nT) z(nT) total(nT) temperature(degC)\n").arg(name, toString(kind), magnetometer->description())); 
    QObject::connect(magnetometer, &Magnetometer::valueChanged, [=](const auto& value) {
        this->valueChanged(name, kind, QVariant::fromValue(value));
    });

    // log new data in dsl format
    QObject::connect(magnetometer, &Magnetometer::valueChanged, [=](const auto& value) {
        sink->write(value.toDslFormat().toUtf8());
    });

}
