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

    Q_SIGNAL void fileNameChanged(QString);
    Q_SIGNAL void bytesWritten(qint64);

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
        if (next_rollover.isNull()) {
            return std::chrono::milliseconds(-1);
        }
        else {
            return next_rollover - QDateTime::currentDateTimeUtc();
        }
    }

    [[nodiscard]] inline auto timeUntilRollover() const noexcept -> int
    {
        return static_cast<int>(timeUntilRolloverAsDuration().count());
    }

    [[nodiscard]] auto isActive() const noexcept -> bool;

    [[nodiscard]] inline auto dir() const noexcept -> QDir
    {
        return dir_;
    }

public slots:

    auto start() -> void;

    auto stop() -> void;

    /// Write a string to the file
    ///
    /// Creates a new file with header information if needed.
    /// \param bytes
    auto write(const QByteArray &bytes) -> void;

    auto setRolloverInterval(const std::chrono::milliseconds& duration) noexcept -> void;

    /// Set the user comment
    ///
    /// The header written to the top of every file
    /// \param header
    auto setHeader(const QString &header) noexcept -> void { header_ = header; }

    auto setHeader(const QStringList &lines) noexcept -> void {
        header_ = lines.join(QChar{'\n'});
    }
    /// Get the user comment
    ///
    /// The header written to the top of every file
    /// \return
    [[nodiscard]] auto header() const noexcept -> QString { return header_; }

    auto flush() noexcept -> void;

private:

    /// Write future data into a new file
    ///
    /// The current file is closed and removed if empty.
    /// The provided datetime will be used for the new filename.
    /// \param datetime
    auto rollover(const QDateTime &datetime) -> void;

    QFile *file = nullptr;
    QDir dir_;
    QString prefix;
    QString suffix;
    QString header_;
    std::chrono::milliseconds rollover_interval;
    QDateTime next_rollover;
};

#endif //MAGGUI_TEXTFILESINK_H
