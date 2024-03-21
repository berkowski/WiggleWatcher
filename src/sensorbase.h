#ifndef SENSORBASE_H
#define SENSORBASE_H

#include <QtCore/qobject.h>

class QIODevice;

class SensorBase : public QObject
{
    Q_OBJECT
public:
    explicit SensorBase(QIODevice *io, QObject *parent = nullptr);

protected slots:
    virtual auto handleReadyRead(QIODevice *) -> void{};

private:
    QIODevice *io;
};

#endif //SENSORBASE_H
