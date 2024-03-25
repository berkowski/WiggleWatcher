#include "sensorbase.h"

#include <QMetaEnum>
#include <QtCore/qiodevice.h>

SensorBase::SensorBase(QIODevice *io_, QObject *parent)
    : QObject(parent)
    , io(io_)
{
    // take ownership of the io device
    io->setParent(this);
    connect(io, &QIODevice::readyRead, [=]() { this->handleReadyRead(io); });
}

auto toString(SensorBase::SensorType t) -> QString
{
    const auto meta = QMetaEnum::fromType<SensorBase::SensorType>();
    return QString{meta.valueToKey(t)};
}

auto fromString(const QString &string) -> SensorBase::SensorType
{
    const auto meta = QMetaEnum::fromType<SensorBase::SensorType>();
    const auto index = meta.keyToValue(string.toUpper().toUtf8().data());
    if (index < 0) {
        return SensorBase::SensorType::UNKNOWN;
    } else {
        return static_cast<SensorBase::SensorType>(index);
    }
}
