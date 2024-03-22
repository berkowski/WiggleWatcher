#ifndef SERIALPORTFACTORY_H
#define SERIALPORTFACTORY_H

#include "maggui-core_global.h"

#include <QtCore/qstring.h>

class QSerialPort;

class MAGGUI_CORE_EXPORT SerialPortFactory
{
public:
    static const QRegularExpression RE;
    static auto from_string(const QString &string) -> QSerialPort *;
};

#endif //SERIALPORTFACTORY_H
