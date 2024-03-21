#ifndef SENSORBASE_H
#define SENSORBASE_H

#include <QtCore/qobject.h>

class QIODevice;

class SensorBase : public QObject
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
