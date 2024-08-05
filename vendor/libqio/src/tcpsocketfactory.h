#ifndef LIBQIO_TCPSOCKETFACTORY_H
#define LIBQIO_TCPSOCKETFACTORY_H

//#include <core/global.h>
#include <libqio/iofactory.h>

#include <QRegularExpression>

class QTcpSocket;

class LIBQIO_EXPORT TcpSocketFactory
{
public:
    static const QRegularExpression RE;
    static auto from_string(const QString &string, IOFactory::ErrorKind *error=nullptr) -> QTcpSocket *;
    static auto to_string(const QTcpSocket *device) -> QString;
    static auto last_error_string() -> QString;
};

#endif //LIBQIO_TCPSOCKETFACTORY_H
