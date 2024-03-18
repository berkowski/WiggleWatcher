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
    udp->bind(address, port);
    return udp;
}
