#include "udpsocketfactory.h"

#include <QtNetwork/qudpsocket.h>

const QRegularExpression UdpSocketFactory::RE = QRegularExpression("udp://([^:]+:)?(\\d+)", QRegularExpression::CaseInsensitiveOption);

auto UdpSocketFactory::from_string(const QString &string) -> QUdpSocket * {
    const auto match = UdpSocketFactory::RE.match(string);
    if (!match.hasMatch()) {
        return nullptr;
    }

    auto address = QHostAddress{QHostAddress::Any};
    if (match.hasCaptured(1)) {
        address = QHostAddress{match.captured(1).remove(QChar(':'))};
    }
    if (address.isNull()) {
        qCritical("local interface: '%s' in udp config: '%s' is invalid.", qUtf8Printable(match.captured(1).remove(QChar(':'))), qUtf8Printable(string));
        return nullptr;
    }

    auto ok = false;
    const auto port = match.captured(2).toInt(&ok);
    if (!ok || (port < 0)) {
        qCritical("local port: '%s' in udp config: '%s' is invalid.", qUtf8Printable(match.captured(2)), qUtf8Printable(string));
        return nullptr;
    }

    auto udp = new QUdpSocket(nullptr);
    // first bind to the local port
    udp->bind(QHostAddress::Any, port, QAbstractSocket::BindFlag::ReuseAddressHint | QAbstractSocket::BindFlag::ShareAddress);
    if(udp->state() != QAbstractSocket::SocketState::BoundState) {
        qCritical("unable to bind on local port: %d.  error: %s", port, qUtf8Printable(udp->errorString()));
        udp->deleteLater();
        return nullptr;
    }
    qCritical() << "AFTER CONNECT: state: " << udp->state() << " remote: " << udp->peerName() << " " << udp->peerPort() << " local: " << udp->localAddress() << " " << udp->localPort() << " error: " << udp->errorString();
    // then "connect" to the remote address/port.
    udp->connectToHost(QStringLiteral("127.0.0.1"), 5556);
    qCritical() << "AFTER CONNECT: state: " << udp->state() << " remote: " << udp->peerName() << " " << udp->peerPort() << " local: " << udp->localAddress() << " " << udp->localPort() << " error: " << udp->errorString();
    if (udp->state() != QAbstractSocket::SocketState::ConnectedState) {
        qCritical("unable to connect to remote: %s:%d.  error: %s", qUtf8Printable("127.0.0.1"), port, qUtf8Printable(udp->errorString()));
        udp->deleteLater();
        return nullptr;
    }

    return udp;
}
