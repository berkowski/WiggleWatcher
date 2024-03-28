#include <core/textfilesink.h>

#include <QtCore/qdir.h>
#include <QtCore/qfile.h>
#include <QtCore/qmap.h>

const QString TextFileSink::DATETIME_FMT = QStringLiteral("yyyyMMdd_HHmmss");

TextFileSink::TextFileSink(const QDir &dir,
                           const QString &prefix,
                           const QString &suffix,
                           QObject *parent)
    : QObject(parent)
    , prefix(prefix)
    , suffix(suffix)
    , dir(dir)
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
    const auto now = QDateTime::currentDateTimeUtc();
    if (now >= next_rollover) {
        rollover(now);
        if (!file->open(QFile::OpenModeFlag::WriteOnly | QFile::OpenModeFlag::Truncate)) {
            qCritical("Unable to open file for writing: '%s'", qUtf8Printable(file->fileName()));
            return;
        }
        // write the header
        file->write(header_.toUtf8());
    }
    file->write(bytes);
}

auto TextFileSink::setRolloverInterval(const std::chrono::milliseconds &duration) noexcept -> void
{
    rollover_interval = duration;
}

auto TextFileSink::filenameForDateTime(const QDateTime &datetime) const -> QString
{
    const auto filename = prefix + datetime.toString(TextFileSink::DATETIME_FMT) + suffix;
    return dir.absoluteFilePath(filename);
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
        file->setFileName(filenameForDateTime(datetime));
        emit filenameChanged(file->fileName());
        const auto intervals_since_epoch = std::chrono::milliseconds(datetime.toMSecsSinceEpoch()) / rollover_interval;
        const auto next_rollover_ms = (intervals_since_epoch + 1) * rollover_interval;
        next_rollover = QDateTime::fromSecsSinceEpoch(std::chrono::duration_cast<std::chrono::seconds>(next_rollover_ms).count());
    }
    else {
        while (next_rollover < datetime) {
            next_rollover += rollover_interval;
        }
        file->setFileName(filenameForDateTime(next_rollover));
        emit filenameChanged(file->fileName());
    }

}

auto TextFileSink::flush() noexcept -> void
{
    if (file->isOpen()) {
        file->flush();
    }
}
