//
// Created by zac on 3/21/24.
//

#include "aps1540magnetometer.h"

#include <QtCore/qiodevice.h>

Aps1540Magnetometer::Aps1540Magnetometer(QIODevice *io, QObject *parent)
    : Magnetometer(io, parent)
{}

void Aps1540Magnetometer::handleReadyRead(QIODevice *device)
{
    while (device->canReadLine()) {
        const auto timestamp = QDateTime::currentDateTimeUtc();

        const auto line = device->readLine();
        emit bytesRead(line);
        // do this for everything - we'll want to print the offending string w/ replacement
        // characters for invalid bytes
        const auto string = QString::fromUtf8(line);
        if (!line.isValidUtf8()) {
            qWarning() << "read bytes that are not a valid utf-8 string: " << string;
            continue;
        }

        auto data = stringToData(string);
        if (data.isNull()) {
            qWarning() << "unable to convert string to data structure: " << string;
            continue;
        }
        data.timestamp = timestamp;
        emit valueChanged(data);
    }

    if (device->bytesAvailable() > 1024) {
        const auto contents = device->readAll();
        qWarning() << "Read " << contents.size()
                   << " bytes without a valid measurement, clearing buffer";
        emit bytesRead(contents);
    }
}
auto Aps1540Magnetometer::stringToData(const QString &string) -> VectorMagnetometerData
{
    return VectorMagnetometerData();
}
