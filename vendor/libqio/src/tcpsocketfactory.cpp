#include "tcpsocketfactory.h"

#include <QTcpSocket>

const QRegularExpression TcpSocketFactory::RE
    = QRegularExpression("tcp://([^:]+):(\\d+)", QRegularExpression::CaseInsensitiveOption);

namespace {
    auto error_string = QString{};
}

auto TcpSocketFactory::from_string(const QString &string, IOFactory::ErrorKind *error) -> QTcpSocket *
{
    if (!string.startsWith("tcp", Qt::CaseInsensitive)) {
        if (error) {
            *error = IOFactory::ErrorKind::IncorrectKind;
        }
        error_string = QStringLiteral("");
        return nullptr;
    };

    const auto match = TcpSocketFactory::RE.match(string);
    if (!match.hasMatch()) {
        if (error) {
            *error = IOFactory::ErrorKind::ConfigParseError;
        }
        error_string = QStringLiteral("TCP config string '%1' is not valid").arg(string);
        qCritical() << error_string;
        return nullptr;
    }

    const auto address = QHostAddress(match.captured(1));
    if (address.isNull()) {
        if (error) {
            *error = IOFactory::ErrorKind::ConfigParseError;
        }
        error_string = QStringLiteral("remote host: '%1' in tcp config: '%2' is invalid.").
                arg(match.captured(1), string);
        qCritical() << error_string;
        return nullptr;
    }
    auto ok = false;
    const auto port = match.captured(2).toInt(&ok);
    if (!ok || (port < 0)) {
        if (error) {
            *error = IOFactory::ErrorKind::ConfigParseError;
        }
        error_string = QStringLiteral("remote port: '%1' in tcp config: '%2' is invalid.")
                  .arg(match.captured(2), string);
        return nullptr;
    }

    // Make a new socket and start connection attempt.
    auto tcp = new QTcpSocket(nullptr);
    tcp->connect(tcp, &QTcpSocket::connected, [address, port]() -> void {
        qInfo("connected to tcp remote %s:%d", qUtf8Printable(address.toString()), port);
    });
    tcp->connectToHost(address, port);
    if (error) {
        *error = IOFactory::ErrorKind::NoError;
    }
    return tcp;
}
auto TcpSocketFactory::to_string(const QTcpSocket *device) -> QString
{
    if (!device) {
        return {};
    }

    return QStringLiteral("tcp://%1:%2").arg(device->peerName()).arg(device->peerPort());
}

auto TcpSocketFactory::last_error_string() -> QString {
    const auto s = error_string;
    error_string.clear();
    return s;
}
