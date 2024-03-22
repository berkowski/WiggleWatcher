#ifndef SENSORBASE_H
#define SENSORBASE_H

#include "maggui-core_global.h"

#include <QtCore/qobject.h>

class QIODevice;

class MAGGUI_CORE_EXPORT SensorBase : public QObject
{
    Q_OBJECT
public:
    Q_SIGNAL void bytesRead(const QByteArray &);

    explicit SensorBase(QIODevice *io, QObject *parent = nullptr);

protected slots:
    virtual auto handleReadyRead(QIODevice *) -> void = 0;

private:
    QIODevice *io;
};

#endif //SENSORBASE_H
