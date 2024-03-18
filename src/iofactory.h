#ifndef IOFACTORY_H
#define IOFACTORY_H

#include <QString>
class QIODevice;

class IOFactory
{
public:
    virtual auto from_string(const QString &string) -> QIODevice *;
};

#endif //IOFACTORY_H
