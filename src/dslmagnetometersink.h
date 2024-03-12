#ifndef DSLMAGNETOMETERSINK_H
#define DSLMAGNETOMETERSINK_H

#include "sinkinterface.h"
#include "types/vectormagnetometerdata.h"

#include <QtCore/qobject.h>
#include <QtCore/qdir.h>
#include <QtCore/qmap.h>
#include <chrono>

class QFile;

class DslMagnetometerSink : public QObject, public SinkInterface {
  Q_OBJECT
  Q_INTERFACES(SinkInterface)
  Q_PROPERTY(QDir path READ path WRITE setPath)
public:
  
  static const QMap<QString, QString> DEFAULT_METADATA;
  static const QString FILENAME_DATETIME_FMT;
  
  DslMagnetometerSink(QDir path = QDir(), QString prefix = QStringLiteral("mag"), QObject *parent = nullptr);

  auto write(const VectorMagnetometerData &record) -> void;

  // Implimentation of SinkInterface
  //
  // Only write data of type VectorMagnetometerData
  auto write(const QVariant &record) -> void override;

  const auto path() noexcept -> QDir
  {
    return m_path;
  }
  
  auto setPath(const QDir& path) -> void
  {
    m_path = path;
  }

  auto flush() -> void
  {
    m_file->flush();
  }
  
  static auto toString(const VectorMagnetometerData &data) -> QString;
  
protected:
  static auto formatMetadata(const QString& key, const QString& value) -> QString;
  auto openNewFileAndWriteMetadata(const QDateTime& timestamp) -> void;
private:
  
  QFile *m_file = nullptr;
  QDir m_path = QDir();
  QString m_prefix = QStringLiteral("mag");
  QMap<QString, QString> m_metadata;
  std::chrono::milliseconds m_file_duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::hours(1));
  QDateTime m_next_file_rollover;
};


#endif
