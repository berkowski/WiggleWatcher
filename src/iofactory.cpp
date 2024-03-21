#include "iofactory.h"
#include "serialportfactory.h"
#include "tcpsocketfactory.h"
#include "udpsocketfactory.h"

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
