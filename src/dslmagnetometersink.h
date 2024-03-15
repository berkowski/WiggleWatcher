#ifndef DSLMAGNETOMETERSINK_H
#define DSLMAGNETOMETERSINK_H

#include "sinkinterface.h"
#include "metadatainterface.h"
#include "types/vectormagnetometerdata.h"

#include <chrono>

#include <QtCore/qdir.h>
#include <QtCore/qmap.h>
#include <QtCore/qobject.h>

class QFile;

class DslMagnetometerSink : public QObject, public SinkInterface, public MetadataInterface
{
    Q_OBJECT
    Q_INTERFACES(SinkInterface MetadataInterface)
    Q_PROPERTY(QDir path READ path WRITE setPath)
public:
    /// Description of file format written in file header
    static const QMap<QString, QString> FORMAT_HEADER;
    static const QString FILENAME_DATETIME_FMT;

    explicit DslMagnetometerSink(const QDir& path = QDir(),
                        QString prefix = QStringLiteral("mag"),
                        QObject *parent = nullptr);

    auto write(const VectorMagnetometerData &record) -> void;

    // Implementation of SinkInterface
    //
    // Only write data of type VectorMagnetometerData
    auto write(const QVariant &record) -> void override;

    // Implementation of MetadataInterface
    auto setValue(const QString &name, QVariant value) -> void override;
    [[nodiscard]] auto value(const QString &name) -> QVariant const override;
    [[nodiscard]] auto values() -> QMap<QString, QVariant> const override;

    [[nodiscard]] auto path() const noexcept -> QDir { return m_path; }

    auto setPath(const QDir &path) -> void { m_path = path; }

    auto flush() -> void { m_file->flush(); }

    static auto toString(const VectorMagnetometerData &data) -> QString;

signals:
    auto metadataChanged(QString name, QVariant value) -> void;

protected:
    static auto formatMetadata(const QString &key, const QString &value) -> QString;
    auto openNewFileAndWriteMetadata(const QDateTime &timestamp) -> void;

private:
    QFile *m_file = nullptr;
    QDir m_path = QDir();
    QString m_prefix = QStringLiteral("mag");
    QMap<QString, QVariant> m_metadata;
    std::chrono::milliseconds m_file_duration
        = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::hours(1));
    QDateTime m_next_file_rollover;
};

#endif
