#include "udpsocketfactory.h"

#include <QUdpSocket>

const QRegularExpression UdpSocketFactory::RE
    = QRegularExpression("udp://([^:]+):(\\d+)(:\\d+)?", QRegularExpression::CaseInsensitiveOption);

namespace {
    auto error_string = QString{};
}

auto UdpSocketFactory::from_string(const QString &string, IOFactory::ErrorKind *error) -> QUdpSocket *
{
    if (!string.startsWith("udp", Qt::CaseInsensitive)) {
        if (error) {
            *error = IOFactory::ErrorKind::IncorrectKind;
        }
        error_string = QStringLiteral("");
        return nullptr;
    };

    const auto match = UdpSocketFactory::RE.match(string);
    if (!match.hasMatch()) {
        if (error) {
            *error = IOFactory::ErrorKind::ConfigParseError;
        }
        error_string = QStringLiteral("UDP config string '%1' is not valid").arg(string);
        qCritical() << error_string;
        return nullptr;
    }

    const auto address = QHostAddress{match.captured(1)};
    if (address.isNull()) {
        if (error) {
            *error = IOFactory::ErrorKind::ConfigParseError;
        }
        error_string = QStringLiteral("local interface: '%1' in udp config: '%2' is invalid.").
                  arg(match.captured(1).remove(QChar(':')), string);
        qCritical() << error_string;
        return nullptr;
    }

    auto ok = false;
    const auto remote_port = match.captured(2).toInt(&ok);
    if (!ok || (remote_port < 0)) {
        if (error) {
            *error = IOFactory::ErrorKind::ConfigParseError;
        }
        error_string = QStringLiteral("port: '%1' in udp config: '%2' is invalid.")
                .arg(match.captured(2), string);
        qCritical() << error_string;
        return nullptr;
    }

    auto local_port = remote_port;
    if (match.hasCaptured(3)) {
        local_port = match.captured(3).remove(QChar(':')).toInt(&ok);
        if (!ok || local_port < 0) {
            if (error) {
                *error = IOFactory::ErrorKind::ConfigParseError;
            }
            error_string = QStringLiteral("port: '%1' in udp config: '%2' is invalid.")
                      .arg(match.captured(3), string);
            qCritical() << error_string;
            return nullptr;
        }
    }

    auto udp = new QUdpSocket(nullptr);
    // first bind to the local port
    udp->bind(local_port, QAbstractSocket::BindFlag::ReuseAddressHint | QAbstractSocket::BindFlag::ShareAddress);
    if (udp->state() != QAbstractSocket::SocketState::BoundState) {
        if (error) {
            *error = IOFactory::ErrorKind::IoError;
        }
        error_string = QStringLiteral("unable to bind on local port: %1.  error: %2")
                  .arg(local_port)
                  .arg(udp->errorString());
        qCritical() << error_string;
        udp->deleteLater();
        return nullptr;
    }
    // then "connect" to the remote address/port.
    udp->connectToHost(address, remote_port);
    if (udp->state() != QAbstractSocket::SocketState::ConnectedState) {
        if (error) {
            *error = IOFactory::ErrorKind::IoError;
        }
        error_string = QStringLiteral("unable to connect to remote: %1:%2.  error: %3")
                  .arg(address.toString())
                  .arg(remote_port)
                  .arg(udp->errorString());
        qCritical() << error_string;
        udp->deleteLater();
        return nullptr;
    }
    if (error) {
        *error = IOFactory::ErrorKind::NoError;
    }
    return udp;
}

auto UdpSocketFactory::to_string(const QUdpSocket *device) -> QString
{
    if (!device) {
        return {};
    }

    const auto remote_port = device->peerPort();
    const auto local_port = device->localPort();

    auto string = QStringLiteral("udp://%1:%2").arg(device->peerName()).arg(remote_port);
    if (remote_port != local_port) {
        string += QStringLiteral(":%1").arg(local_port);
    }
    return string;
}

auto UdpSocketFactory::last_error_string() -> QString {
    const auto s = error_string;
    error_string.clear();
    return s;
}
