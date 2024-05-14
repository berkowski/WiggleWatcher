#ifndef SENSORBASE_H
#define SENSORBASE_H

#include <core/global.h>
#include <core/sensorkind.h>

#include <QtCore/qmetatype.h>
#include <QtCore/qobject.h>
#include <QtCore/qstring.h>

class QIODevice;

class MAGGUI_CORE_EXPORT SensorBase : public QObject {
Q_OBJECT

public:
    Q_SIGNAL void bytesRead(const QByteArray &);

    explicit SensorBase(QIODevice *io, QObject *parent = nullptr);

    [[nodiscard]] virtual auto type() const noexcept -> maggui::SensorKind = 0;

    [[nodiscard]] auto name() const noexcept -> QString {
        return objectName();
    }

    [[nodiscard]] auto description() const noexcept -> QString {
        return description_;
    }

public slots:

    auto setDescription(QString description) -> void {
        description_ = std::move(description);
    };

    auto setName(QString name) -> void {
        setObjectName(name);
    }

protected slots:

    virtual auto handleReadyRead(QIODevice *) -> void = 0;

private:
    QIODevice *io;
    QString description_;
};
#endif //SENSORBASE_H
