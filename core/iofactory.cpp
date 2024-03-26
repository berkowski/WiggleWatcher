#include "serialportfactory.h"
#include "tcpsocketfactory.h"
#include "udpsocketfactory.h"
#include <core/iofactory.h>

#include <QtNetwork/qtcpsocket.h>
#include <QtNetwork/qudpsocket.h>
#include <QtSerialPort/qserialport.h>

auto IOFactory::from_string(const QString &string) -> QIODevice *
{
    QIODevice *io = nullptr;
    io = SerialPortFactory::from_string(string);
    if (io) {
        return io;
    }

    io = UdpSocketFactory::from_string(string);
    if (io) {
        return io;
    }

    io = TcpSocketFactory::from_string(string);
    if (io) {
        return io;
    }
    return nullptr;
}
auto IOFactory::to_string(const QIODevice *device) -> QString
{
    if (auto d = qobject_cast<const QSerialPort *>(device)) {
        return SerialPortFactory::to_string(d);
    }

    if (auto d = qobject_cast<const QUdpSocket *>(device)) {
        return UdpSocketFactory::to_string(d);
    }

    if (auto d = qobject_cast<const QTcpSocket *>(device)) {
        return TcpSocketFactory::to_string(d);
    }
    return {};
}
