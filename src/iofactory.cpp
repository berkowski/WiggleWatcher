#include "iofactory.h"

#include <QtNetwork/qudpsocket.h>
#include <QtNetwork/qtcpsocket.h>
#include <QtSerialPort/qserialport.h>

class UdpSocketFactory {
public:
    static auto from_config() -> QUdpSocket* {
        return nullptr;
    }
};

class TcpSocketFactory {
public:
    static auto from_config() -> QTcpSocket* {
        return nullptr;
    }
};

class SerialPortFactory {
public:
    static auto from_config() -> QSerialPort* {
        return nullptr;
    }
};

auto IOFactory::from_config() -> QIODevice*
{
    return UdpSocketFactory::from_config();
}
