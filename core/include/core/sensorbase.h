#ifndef SENSORBASE_H
#define SENSORBASE_H

#include "global.h"

#include "../../../../../../../../gnu/store/wq488jkxrfswnsx7f0r8fxfywr17wqdr-profile/include/qt6/QtCore/qmetatype.h"
#include "../../../../../../../../gnu/store/wq488jkxrfswnsx7f0r8fxfywr17wqdr-profile/include/qt6/QtCore/qobject.h"

class QIODevice;

class MAGGUI_CORE_EXPORT SensorBase : public QObject
{
    Q_OBJECT
public:
    enum SensorType {
        UNKNOWN = 0,
        APS1540,
        HMR2300,
        USER_TYPE,
    };
    Q_ENUM(SensorType)

    Q_SIGNAL void bytesRead(const QByteArray &);
    explicit SensorBase(QIODevice *io, QObject *parent = nullptr);

    [[nodiscard]] virtual auto type() const noexcept -> SensorType = 0;

protected slots:
    virtual auto handleReadyRead(QIODevice *) -> void = 0;

private:
    QIODevice *io;
};

MAGGUI_CORE_EXPORT auto toString(SensorBase::SensorType t) -> QString;
MAGGUI_CORE_EXPORT auto fromString(const QString &string) -> SensorBase::SensorType;

#endif //SENSORBASE_H
