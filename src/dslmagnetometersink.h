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

public:
  
  static const QMap<QString, QString> DEFAULT_METADATA;
  
  DslMagnetometerSink(QDir path = QDir(), QString prefix = QStringLiteral("mag"), QObject *parent = nullptr);

  auto write(const VectorMagnetometerData &record) -> void;

  // Implimentation of SinkInterface
  //
  // Only write data of type VectorMagnetometerData
  auto write(const QVariant &record) -> void override;

  static auto toString(const VectorMagnetometerData &data) -> QString;
  
  static auto nextRolloverMS(const std::chrono::milliseconds& now, const std::chrono::milliseconds& duration) -> std::chrono::milliseconds;

protected:
  static auto formatMetadata(const QString& key, const QString& value) -> QString;
  
private:
  
  QFile *m_file = nullptr;
  QDir m_path = QDir();
  QString m_prefix = QStringLiteral("mag");
  QMap<QString, QString> m_metadata;
  std::chrono::milliseconds m_file_duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::hours(1));
  std::chrono::milliseconds m_next_file_rollover;
};


#endif
