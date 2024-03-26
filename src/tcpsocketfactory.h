#ifndef MAGGUI_TCPSOCKETFACTORY_H
#define MAGGUI_TCPSOCKETFACTORY_H

#include "maggui-core_global.h"

#include <QtCore/qregularexpression.h>

class QTcpSocket;

class MAGGUI_CORE_EXPORT TcpSocketFactory
{
public:
    static const QRegularExpression RE;
    static auto from_string(const QString &string) -> QTcpSocket *;
    static auto to_string(const QTcpSocket* device) -> QString;
};

#endif //MAGGUI_TCPSOCKETFACTORY_H
