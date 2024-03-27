#ifndef MAGGUI_TEXTFILESINK_H
#define MAGGUI_TEXTFILESINK_H

#include <core/global.h>

#include <QtCore/qdir.h>
#include <QtCore/qobject.h>
#include <QtCore/qstring.h>
#include <QtCore/qdatetime.h>

#include <chrono>

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

    [[nodiscard]] inline auto rolloverIntervalAsDuration() const noexcept -> std::chrono::milliseconds
    {
        return rollover_interval;
    }

    [[nodiscard]] inline auto rolloverInterval() const noexcept -> int
    {
        return static_cast<int>(rollover_interval.count());
    }

    [[nodiscard]] inline auto timeUntilRolloverAsDuration() const noexcept -> std::chrono::milliseconds
    {
        return QDateTime::currentDateTimeUtc() - next_rollover;
    }

    [[nodiscard]] inline auto timeUntilRollover() const noexcept -> int
    {
        return timeUntilRolloverAsDuration().count();
    }
public slots:
    /// Write a string to the file
    ///
    /// Creates a new file with header information if needed.
    /// \param bytes
    auto write(const QByteArray &bytes) -> void;

    auto setRolloverInterval(const std::chrono::milliseconds& duration) noexcept -> void;

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

    auto flush() noexcept -> void;

private:
    QFile *file = nullptr;
    QDir dir;
    QString prefix;
    QString suffix;
    QMap<QString, QString> metadata;
    QString userComment_;
    std::chrono::milliseconds rollover_interval;
    QDateTime next_rollover;
};

#endif //MAGGUI_TEXTFILESINK_H
