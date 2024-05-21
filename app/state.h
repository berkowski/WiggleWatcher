#ifndef MAGGUI_STATE_H
#define MAGGUI_STATE_H

#include <QtCore/qmetatype.h>
#include <QtCore/qobject.h>

namespace maggui
{
  struct State {
    State();
    bool recording;
    QString log_directory;
  };
}

Q_DECLARE_METATYPE(maggui::State)


class StateObject: public QObject {
 Q_OBJECT

 public:
  explicit StateObject(QObject* parent=nullptr, QWidget* widget=nullptr);

  [[nodiscard]] auto isRecordingEnabled() const noexcept -> bool {
    return state.recording;
  }

  [[nodiscard]] auto currentState() const noexcept -> maggui::State {
    return state;
  }
  
 public slots:
   auto setRecordingEnabled(bool enabled) noexcept -> void;
   auto toggleRecordingEnabled() noexcept -> void;
   auto setLogDirectory(QString directory) noexcept -> void;

 signals:
  auto stateChanged(maggui::State)->void;
  
 private:
  maggui::State state;
  QWidget* widget;
};
  

#endif
