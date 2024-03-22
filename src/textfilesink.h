#ifndef MAGGUI_TEXTFILESINK_H
#define MAGGUI_TEXTFILESINK_H

#include "maggui-core_global.h"

#include <QtCore/qdir.h>
#include <QtCore/qobject.h>
#include <QtCore/qstring.h>

class QFile;
class QDir;
class QString;
class QDateTime;

class MAGGUI_CORE_EXPORT TextFileSink : public QObject
{
    Q_OBJECT
public:
    static const QString DATETIME_FMT;

    explicit TextFileSink(const QDir &dir = QDir{},
                          const QString &prefix = QStringLiteral("raw"),
                          const QString &suffix = QStringLiteral(".txt"),
                          QObject *parent = nullptr);

    /// Return the current filename
    ///
    /// \returns an empty string if there is no file currently open.
    [[nodiscard]] auto currentFileName() const -> QString;

    /// Construct a new filename for the provided datetime
    ///
    /// \param datetime
    /// \return
    [[nodiscard]] auto filenameForDateTime(const QDateTime &datetime) const -> QString;

public slots:
    /// Write a string to the file
    ///
    /// Creates a new file with header information if needed.
    /// \param bytes
    auto write(const QByteArray &bytes) -> void;

    /// Write future data into a new file
    ///
    /// The current file is closed and removed if empty.
    /// The provided datetime will be used for the new filename.
    /// \param datetime
    auto rollover(const QDateTime &datetime) -> void;

    /// Set the user comment
    ///
    /// The user comment is written after the metadata fields in the header of each file
    /// \param comment
    auto setUserComment(const QString &comment) noexcept -> void { userComment_ = comment; }

    /// Get the user comment
    ///
    /// The user comment is written after the metadata fields in the header of each file
    /// \return
    [[nodiscard]] auto userComment() const noexcept -> QString { return userComment_; }

private:
    QFile *file = nullptr;
    QDir dir;
    QString prefix;
    QString suffix;
    QMap<QString, QString> metadata;
    QString userComment_;
};

#endif //MAGGUI_TEXTFILESINK_H
