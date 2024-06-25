#include "serialportfactory.h"
#include "tcpsocketfactory.h"
#include "udpsocketfactory.h"
#include <core/iofactory.h>

#include <QTcpSocket>
#include <QUdpSocket>
#include <QSerialPort>

namespace {
    auto error_string = QString{};
}

auto IOFactory::from_string(const QString &string, ErrorKind *error) -> QIODevice *
{
    QIODevice *io = nullptr;
    ErrorKind local_error = ErrorKind::NoError;
    io = SerialPortFactory::from_string(string, &local_error);
    if (io) {
        return io;
    }
    else if (local_error != ErrorKind::IncorrectKind) {
        if (error) {
            *error = local_error;
        }
        error_string = SerialPortFactory::last_error_string();
        return nullptr;
    }

    io = UdpSocketFactory::from_string(string, &local_error);
    if (io) {
        return io;
    }
    else if (local_error != ErrorKind::IncorrectKind) {
        if (error) {
            *error = local_error;
        }
        error_string = UdpSocketFactory::last_error_string();
        return nullptr;
    }

    io = TcpSocketFactory::from_string(string, &local_error);
    if (io) {
        return io;
    }
    else if (local_error != ErrorKind::IncorrectKind) {
        if (error) {
            *error = local_error;
        }
        error_string = TcpSocketFactory::last_error_string();
        return nullptr;
    }

    if(error) {
        *error = ErrorKind::ConfigParseError;
    }
    error_string = QStringLiteral("Invalid connection config: %1").arg(string);
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

auto IOFactory::last_error_string() -> QString {
    const auto s = error_string;
    error_string.clear();
    return s;
}
