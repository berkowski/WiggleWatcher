#ifndef IOFACTORY_H
#define IOFACTORY_H

class QIODevice;

class IOFactory
{
public:
    virtual auto from_config() -> QIODevice*;
};

#endif //IOFACTORY_H
