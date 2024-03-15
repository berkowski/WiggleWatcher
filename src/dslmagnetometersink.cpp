
#include "dslmagnetometersink.h"

#include <utility>
#include <QtCore/qdir.h>
#include <QtCore/qfile.h>

const QMap<QString, QString> DslMagnetometerSink::DEFAULT_METADATA
    = {{"Description", ""},
       {"Field 1", "timestamp, ISO format (YYYY-MM-DDTHH:mm:ss.sss)"},
       {"Field 2", "magnetic field x-component (nT)"},
       {"Field 3", "magnetic field y-component (nT)"},
       {"Field 4", "magnetic field z-component (nT)"},
       {"Field 5", "instrumen temperature (degrees C)"}};

const QString DslMagnetometerSink::FILENAME_DATETIME_FMT = QStringLiteral("yyyyMMdd_HHmmss");

DslMagnetometerSink::DslMagnetometerSink(const QDir& path, QString prefix, QObject *parent)
    : QObject(parent)
    , m_path(path)
    , m_prefix(std::move(std::move(prefix)))
    , m_metadata(DEFAULT_METADATA)
    , m_file(new QFile(this))
{}

auto DslMagnetometerSink::write(const QVariant &record) -> void
{
    // Only record vector maggie data
    if (record.userType() == qMetaTypeId<VectorMagnetometerData>()) {
        write(qvariant_cast<VectorMagnetometerData>(record));
    }
}

auto DslMagnetometerSink::openNewFileAndWriteMetadata(const QDateTime &timestamp) -> void
{
    m_file->close();
    if (!m_path.exists()) {
        m_path.mkpath(".");
    }
    const auto filename
        = QStringLiteral("%1_%2.dat").arg(m_prefix, timestamp.toString(FILENAME_DATETIME_FMT));
    m_file->setFileName(m_path.filePath(filename));
    m_file->open(QIODevice::WriteOnly | QIODevice::Text);
    // Write metadata to file
    std::for_each(m_metadata.constKeyValueBegin(),
                  m_metadata.constKeyValueEnd(),
                  [this](const auto &it) {
                      this->m_file->write(
                          DslMagnetometerSink::formatMetadata(it.first, it.second).toUtf8());
                  });
}

auto DslMagnetometerSink::write(const VectorMagnetometerData &record) -> void
{
    if (record.timestamp >= m_next_file_rollover) {
        m_file->close();
        if (!m_next_file_rollover.isNull()) {
            m_next_file_rollover += m_file_duration;
            openNewFileAndWriteMetadata(m_next_file_rollover);
        } else {
            m_next_file_rollover = QDateTime::fromMSecsSinceEpoch(
                (record.timestamp.toMSecsSinceEpoch() / m_file_duration.count() + 1)
                * m_file_duration.count());
            openNewFileAndWriteMetadata(record.timestamp);
        }
    }
    // write the data to disk
    m_file->write(DslMagnetometerSink::toString(record).toUtf8());
}

auto DslMagnetometerSink::toString(const VectorMagnetometerData &data) -> QString
{
    return QStringLiteral("%1 %2 %3 %4 %5\n")
        .arg(data.timestamp.toString(Qt::ISODateWithMs), 20)
        .arg(data.x, 10)
        .arg(data.y, 10)
        .arg(data.z, 10)
        .arg(data.temperature, 7, 'f', 3);
}

auto DslMagnetometerSink::formatMetadata(const QString &key, const QString &value) -> QString
{
    return QStringLiteral("#%1: %2\n").arg(key, value);
}
