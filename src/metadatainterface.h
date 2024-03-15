#ifndef MAGGUI_METADATAINTERFACE_H
#define MAGGUI_METADATAINTERFACE_H

#include <QtCore/qobject.h>
#include <QtCore/qvariant.h>

class MetadataInterface
{
public:
    virtual ~MetadataInterface() = default;

    /// Set a metadata value
    ///
    /// If a value associated with the given name already exists it should be
    /// overwritten with the new value
    /// \param name
    /// \param value
    virtual auto setValue(const QString& name, QVariant value) -> void = 0;

    /// Get a metadata value
    ///
    /// Get the value of the metadata item associated with the given name.
    /// \param name
    /// \return
    [[nodiscard]] virtual auto value(const QString& name) -> QVariant const = 0;

    /// Get all metadata values
    ///
    /// \return
    [[nodiscard]] virtual auto values() -> QMap<QString, QVariant> const = 0;

signals:
    /// Signal emitted when the sensor has a new data record
    virtual auto metadataChanged(QString name, QVariant value) -> void = 0;
};

Q_DECLARE_INTERFACE(MetadataInterface, "maggui.metadatainterface/1.0")
#endif //MAGGUI_METADATAINTERFACE_H
