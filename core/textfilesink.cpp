#include <core/textfilesink.h>

#include <QDir>
#include <QFile>
#include <QMap>
#include <QTimeZone>

const QString TextFileSink::DATETIME_FMT = QStringLiteral("yyyyMMdd_HHmmss");

TextFileSink::TextFileSink(const QDir &dir,
                           const QString &prefix,
                           const QString &suffix,
                           QObject *parent)
    : QObject(parent)
    , prefix(prefix)
    , suffix(suffix)
    , dir_(dir)
    , file(new QFile(this))
    , rollover_interval(std::chrono::hours(1))
{
    if (!prefix.isEmpty() && !prefix.endsWith("_")) {
        this->prefix = prefix + QStringLiteral("_");
    }
    if (!suffix.isEmpty() && !suffix.startsWith(".")) {
        this->suffix = QStringLiteral(".") + suffix;
    }
}

auto TextFileSink::currentFileName() const -> QString
{
    return file->fileName();
}

auto TextFileSink::write(const QByteArray &bytes) -> void
{
    if(!isActive()) {
        return;
    }

    const auto now = QDateTime::currentDateTimeUtc();
    if (now >= next_rollover) {
        rollover(now);
    }
    if (!file->isOpen()) {
        if (!file->open(QFile::OpenModeFlag::WriteOnly | QFile::OpenModeFlag::Truncate)) {
            qCritical("Unable to open file for writing: '%s'", qUtf8Printable(file->fileName()));
            return;
        }
        // write the header
        const auto header_bytes = header_.toUtf8();
        const auto bytes_written = file->write(header_bytes);
        if(bytes_written != header_bytes.size())
        {
            qWarning() << "Short write to file '" << file->fileName() << "', wrote " << bytes_written << " bytes, expected " << bytes.size();
        }
        emit bytesWritten(bytes_written);
    }

    const auto bytes_written = file->write(bytes);
    emit bytesWritten(bytes_written);

    if(bytes_written != bytes.size())
    {
        qWarning() << "Short write to file '" << file->fileName() << "', wrote " << bytes_written << " bytes, expected " << bytes.size();
    }
    file->flush();
}

auto TextFileSink::setRolloverInterval(const std::chrono::milliseconds &duration) noexcept -> void
{
    rollover_interval = duration;
}

auto TextFileSink::filenameForDateTime(const QDateTime &datetime) const -> QString
{
    const auto filename = prefix + datetime.toString(TextFileSink::DATETIME_FMT) + suffix;
    return dir_.absoluteFilePath(filename);
}

auto TextFileSink::rollover(const QDateTime &datetime) -> void
{
    // close the file if open, deleting it as well if empty
    if (file->isOpen()) {
        file->close();
        if (file->size() == 0 && !file->remove()) {
            qWarning() << "Unable to remove empty file: '%s'" << file->fileName();
        }
    }

    // calculate the new filename
    if (next_rollover.isNull()) {
        qDebug() << "next rollover is NULL, using " << datetime << "for new filename.";
        file->setFileName(filenameForDateTime(datetime));
        emit fileNameChanged(file->fileName());
        const auto intervals_since_epoch = std::chrono::milliseconds(datetime.toMSecsSinceEpoch()) / rollover_interval;
        const auto next_rollover_ms = (intervals_since_epoch + 1) * rollover_interval;
        next_rollover = QDateTime::fromSecsSinceEpoch(std::chrono::duration_cast<std::chrono::seconds>(next_rollover_ms).count(), QTimeZone::utc());
    }
    else {
        while (next_rollover < datetime) {
            next_rollover += rollover_interval;
        }
        file->setFileName(filenameForDateTime(next_rollover));
        emit fileNameChanged(file->fileName());
    }
    qDebug() << "next rollover set to: " << next_rollover;
}

auto TextFileSink::flush() noexcept -> void
{
    if (file->isOpen()) {
        file->flush();
    }
}
auto TextFileSink::isActive() const noexcept -> bool
{
    return !next_rollover.isNull();
}

auto TextFileSink::start() -> void
{
    rollover(QDateTime::currentDateTimeUtc());
}

auto TextFileSink::stop() -> void
{
    if(file->isOpen())
    {
        file->close();
        if (file->size() == 0 && !file->remove()) {
            qWarning() << "Unable to remove empty file: '%s'" << file->fileName();
        }
    }
    file->setFileName({});
    next_rollover = QDateTime{};
}

auto TextFileSink::setDir(QDir d) -> void
{
    if (d != dir()) {
        const auto restart = isActive();
        stop();
        dir_ = std::move(d);
        if (restart) {
            start();
        }
    }
}