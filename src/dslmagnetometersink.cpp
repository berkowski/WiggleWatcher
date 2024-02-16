#include "dslmagnetometersink.h"

#include <QtCore/qfile.h>
#include <chrono>

const QMap<QString, QString> DslMagnetometerSink::DEFAULT_METADATA = {
    {"Description", ""},
    {"Field 1", "timestamp, ISO format (YYYY-MM-DDTHH:mm:ss.sss)"},
    {"Field 2", "magnetic field x-component (nT)"},
    {"Field 3", "magnetic field y-component (nT)"},
    {"Field 4", "magnetic field z-component (nT)"},
    {"Field 5", "instrumen temperature (degrees C)"}};

DslMagnetometerSink::DslMagnetometerSink(QDir path, QString prefix, QObject *parent)
: QObject(parent), m_path(path), m_prefix(prefix), m_metadata(DEFAULT_METADATA), m_file(new QFile(this)) {}

auto DslMagnetometerSink::write(const QVariant &record) -> void {
  // Only record vector maggie data
  if (record.userType() == qMetaTypeId<VectorMagnetometerData>()) {
    write(qvariant_cast<VectorMagnetometerData>(record));
  }
}

auto DslMagnetometerSink::write(const VectorMagnetometerData &record) -> void {

  // Write metadata to file
  std::for_each(m_metadata.constKeyValueBegin(), m_metadata.constKeyValueEnd(),
                [this](const auto &it) {
                  this->m_file->write(
                                      DslMagnetometerSink::formatMetadata(it.first, it.second).toUtf8());
                });
  
                
                
  // write the data to disk
  m_file->write(DslMagnetometerSink::toString(record).toUtf8());
}

auto DslMagnetometerSink::toString(const VectorMagnetometerData & data) -> QString {
  return QStringLiteral("%1 %2 %3 %4 %5\n")
      .arg(data.timestamp.toString(Qt::ISODateWithMs), 20)
      .arg(data.x, 10)
      .arg(data.y, 10)
      .arg(data.z, 10)
      .arg(data.temperature, 7, 'f', 3);
}

auto DslMagnetometerSink::formatMetadata(const QString &key,
                                         const QString &value) -> QString {
  return QStringLiteral("#%1: %2\n").arg(key).arg(value);
}

auto DslMagnetometerSink::nextRolloverMS(
    const std::chrono::milliseconds &now,
                                         const std::chrono::milliseconds& duration) -> std::chrono::milliseconds {
  const auto whole = now / duration;
  return (whole + 1) * duration;
}
