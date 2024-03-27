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
    if (file->isOpen()) {
        return file->fileName();
    }
    return {};
}

auto TextFileSink::write(const QByteArray &bytes) -> void
{
    const auto now = QDateTime::currentDateTimeUtc();
    auto write_header = false;
    if (now >= next_rollover) {
        auto next = QDateTime{};
        if (file->fileName().isEmpty()) {
            file->setFileName(filenameForDateTime(now));
            next = QDateTime::fromSecsSinceEpoch(std::chrono::duration_cast<std::chrono::seconds>(((std::chrono::milliseconds(now.toMSecsSinceEpoch()) / rollover_interval) + 1) * rollover_interval).count());
        }
        else {
            file->close();
            if (file->size() == 0) {
                if (!file->remove()) {
                    qWarning() << "Unable to remove empty file: '%s'" << file->fileName();
                }
            }
            file->setFileName(filenameForDateTime(next_rollover));
            next = next_rollover + rollover_interval;
        }

        if (!file->open(QFile::OpenModeFlag::WriteOnly | QFile::OpenModeFlag::Truncate)) {
            qCritical("Unable to open file for writing: '%s'", qUtf8Printable(file->fileName()));
            return;
        }
        write_header = true;
        next_rollover = next;
    }

    if (write_header) {
        // write header
        std::for_each(metadata.constKeyValueBegin(),
                      metadata.constKeyValueEnd(),
                      [&](const auto &it) {
                          file->write(QStringLiteral("#%1: %2").arg(it.first, it.second).toUtf8());
                      });

        // write format comments, if needed
        if (!userComment_.isEmpty()) {
            file->write(userComment_.toUtf8());
        }
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
    const auto next_filename = filenameForDateTime(datetime);
    // clean up current file if open
    if (file->isOpen()) {
        file->flush();
        file->close();
        // if it's empty, remove it
        if (file->size() == 0) {
            if (!file->remove()) {
                qWarning() << "Unable to remove empty file: '%s'" << file->fileName();
            }
        }
    }
    // set the new filename but don't open it yet, that'll happen on the first call to 'write'
    file->setFileName(next_filename);
}

auto TextFileSink::flush() noexcept -> void
{
    if (file->isOpen()) {
        file->flush();
    }
}
