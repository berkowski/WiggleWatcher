#include "aps1540magnetometer.h"

#include <QtCore/qiodevice.h>
#include <QtCore/qregularexpression.h>

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
    const auto fields = string.simplified().split(' ', Qt::SkipEmptyParts);
    if (fields.size() != 4) {
        return {};
    }
    auto ok = true;
    const auto GAUSS_TO_NANO_TESLA = 100000.0;
    const auto x = fields.at(0).toDouble(&ok) * GAUSS_TO_NANO_TESLA;
    if (!ok) {
        return {};
    }
    const auto y = fields.at(1).toDouble(&ok) * GAUSS_TO_NANO_TESLA;
    if (!ok) {
        return {};
    }
    const auto z = fields.at(2).toDouble(&ok) * GAUSS_TO_NANO_TESLA;
    if (!ok) {
        return {};
    }
    const auto temperature = fields.at(3).toDouble(&ok);
    if (!ok) {
        return {};
    }

    return VectorMagnetometerData{QDateTime{}, x, y, z, temperature};
}
