#ifndef MAGGUI_UDPSOCKETFACTORY_H
#define MAGGUI_UDPSOCKETFACTORY_H

#include <QtCore/qregularexpression.h>

class QUdpSocket;

class UdpSocketFactory
{
public:
    static const QRegularExpression RE;
    static auto from_string(const QString &string) -> QUdpSocket *;
};

#endif //MAGGUI_UDPSOCKETFACTORY_H
