#include "udpsocketfactory.h"

#include <QUdpSocket>

const QRegularExpression UdpSocketFactory::RE
    = QRegularExpression("udp://([^:]+):(\\d+)(:\\d+)?", QRegularExpression::CaseInsensitiveOption);

auto UdpSocketFactory::from_string(const QString &string) -> QUdpSocket *
{
    const auto match = UdpSocketFactory::RE.match(string);
    if (!match.hasMatch()) {
        return nullptr;
    }

    const auto address = QHostAddress{match.captured(1)};
    if (address.isNull()) {
        qCritical("local interface: '%s' in udp config: '%s' is invalid.",
                  qUtf8Printable(match.captured(1).remove(QChar(':'))),
                  qUtf8Printable(string));
        return nullptr;
    }

    auto ok = false;
    const auto remote_port = match.captured(2).toInt(&ok);
    if (!ok || (remote_port < 0)) {
        qCritical("port: '%s' in udp config: '%s' is invalid.",
                  qUtf8Printable(match.captured(2)),
                  qUtf8Printable(string));
        return nullptr;
    }

    auto local_port = remote_port;
    if (match.hasCaptured(3)) {
        local_port = match.captured(3).remove(QChar(':')).toInt(&ok);
        if (!ok || local_port < 0) {
            qCritical("port: '%s' in udp config: '%s' is invalid.",
                      qUtf8Printable(match.captured(3)),
                      qUtf8Printable(string));
            return nullptr;
        }
    }

    auto udp = new QUdpSocket(nullptr);
    // first bind to the local port
    udp->bind(local_port, QAbstractSocket::BindFlag::ReuseAddressHint | QAbstractSocket::BindFlag::ShareAddress);
    if (udp->state() != QAbstractSocket::SocketState::BoundState) {
        qCritical("unable to bind on local port: %d.  error: %s",
                  local_port,
                  qUtf8Printable(udp->errorString()));
        udp->deleteLater();
        return nullptr;
    }
    // then "connect" to the remote address/port.
    udp->connectToHost(address, remote_port);
    if (udp->state() != QAbstractSocket::SocketState::ConnectedState) {
        qCritical("unable to connect to remote: %s:%d.  error: %s",
                  qUtf8Printable(address.toString()),
                  remote_port,
                  qUtf8Printable(udp->errorString()));
        udp->deleteLater();
        return nullptr;
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
