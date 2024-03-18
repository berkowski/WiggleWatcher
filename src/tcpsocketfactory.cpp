#include "tcpsocketfactory.h"

#include <QtNetwork/qtcpsocket.h>

const QRegularExpression TcpSocketFactory::RE = QRegularExpression("tcp://([^:]+):(\\d+)", QRegularExpression::CaseInsensitiveOption);

auto TcpSocketFactory::from_string(const QString &string) -> QTcpSocket * {
    const auto match = TcpSocketFactory::RE.match(string);
    if (!match.hasMatch()) {
        return nullptr;
    }

    const auto address = QHostAddress(match.captured(1));
    if (address.isNull()) {
        qCritical("remote host: '%s' in tcp config: '%s' is invalid.", qUtf8Printable(match.captured(1)), qUtf8Printable(string));
        return nullptr;
    }
    auto ok = false;
    const auto port = match.captured(2).toInt(&ok);
    if (!ok || (port < 0)) {
        qCritical("remote port: '%s' in tcp config: '%s' is invalid.", qUtf8Printable(match.captured(2)), qUtf8Printable(string));
        return nullptr;
    }

    // Make a new socket and start connection attempt.
    auto tcp = new QTcpSocket(nullptr);
    tcp->connect(tcp, &QTcpSocket::connected, [address, port]()->void {
        qInfo("connected to tcp remote %s:%d", qUtf8Printable(address.toString()), port);
    });
    tcp->connectToHost(address, port);
    return tcp;
}
