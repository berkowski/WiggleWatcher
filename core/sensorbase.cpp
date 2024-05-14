#include <core/sensorbase.h>

#include <QtCore/qiodevice.h>
#include <QtCore/qmetaobject.h>

SensorBase::SensorBase(QIODevice *io_, QObject *parent)
    : QObject(parent)
    , io(io_)
{
    // take ownership of the io device
    io->setParent(this);
    connect(io, &QIODevice::readyRead, [=]() { this->handleReadyRead(io); });
}
