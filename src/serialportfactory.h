#ifndef SERIALPORTFACTORY_H
#define SERIALPORTFACTORY_H

#include <QtCore/qstring.h>

class QSerialPort;

class SerialPortFactory
{
public:
    static const QRegularExpression RE;
    static auto from_string(const QString &string) -> QSerialPort *;
};

#endif //SERIALPORTFACTORY_H
