#ifndef MAGGUI_TCPSOCKETFACTORY_H
#define MAGGUI_TCPSOCKETFACTORY_H

#include <QtCore/qregularexpression.h>

class QTcpSocket;

class TcpSocketFactory
{
public:
    static const QRegularExpression RE;
    static auto from_string(const QString &string) -> QTcpSocket *;
};

#endif //MAGGUI_TCPSOCKETFACTORY_H
