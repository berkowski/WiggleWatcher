#include "state.h"
#include <QtCore/qdir.h>

namespace maggui {
  State::State(): recording(false), log_directory(QDir::currentPath()) {}
}

StateObject::StateObject(QObject* parent): QObject(parent) {}; 

auto StateObject::setRecordingEnabled(bool enabled) noexcept -> void {
  qDebug() << "setRecordingEnabled: " << std::boolalpha << enabled;
  if (enabled != state.recording) {
    state.recording = enabled;
    emit stateChanged(state);
  }
}

auto StateObject::setLogDirectory(QString directory) noexcept -> void {
  if (directory != state.log_directory) {
    state.log_directory = directory;
    emit stateChanged(state);
  }
}
