#ifndef WIGGLEWATCHER_STATE_H
#define WIGGLEWATCHER_STATE_H

#include <QMetaType>
#include <QObject>
#include <QStorageInfo>

class QTimer;

namespace wigglewatcher
{
  struct State {
    State();
    bool recording;
    QString log_directory;
    qint64 bytes_available;
    qint64 data_out_rate_Bps;
    qint64 data_in_rate_Bps;
    float sample_rate_hz;
  };
}

Q_DECLARE_METATYPE(wigglewatcher::State)


class StateObject: public QObject {
 Q_OBJECT

 public:
  explicit StateObject(QObject* parent=nullptr, QWidget* widget=nullptr);

  [[nodiscard]] auto isRecordingEnabled() const noexcept -> bool {
    return state.recording;
  }

  [[nodiscard]] auto currentState() const noexcept -> wigglewatcher::State {
    return state;
  }
  
 public slots:
   auto setRecordingEnabled(bool enabled) noexcept -> void;
   auto toggleRecordingEnabled() noexcept -> void;
   auto setLogDirectory(QString directory) noexcept -> void;
   auto updateBytesWritten(qint64 bytes) noexcept -> void;
   auto updateBytesRead(qint64 bytes) noexcept -> void;
   auto updateSampleCount(qint64 samples) noexcept -> void;

 signals:
  auto stateChanged(wigglewatcher::State)->void;

 private slots:
   auto handleTimerUpdate() noexcept -> void; 
 private:
  wigglewatcher::State state;
  QWidget* widget;
  QTimer* timer;
  QStorageInfo storage_info;
  qint64 bytes_written_since_last;
  qint64 bytes_read_since_last;
  qint64 samples_since_last;
};
  

#endif
