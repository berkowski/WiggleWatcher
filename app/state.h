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
    qint64 data_rate_Bps;
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
};
  

#endif
