#include "state.h"
#include <QtCore/qdir.h>
#include <QtWidgets/qmessagebox.h>

namespace maggui {
  State::State(): recording(false), log_directory(QDir::currentPath()) {}
}

StateObject::StateObject(QObject* parent, QWidget* widget_): QObject(parent), widget(widget_) {};

auto StateObject::setRecordingEnabled(bool enabled) noexcept -> void {
  if (enabled != state.recording) {
    state.recording = enabled;
    emit stateChanged(state);
  }
}

auto StateObject::setLogDirectory(QString directory) noexcept -> void {
  if (directory != state.log_directory) {
    if (QFileInfo(directory).isWritable()) {
       state.log_directory = directory;
       emit stateChanged(state);
    }
    else {
	    qWarning() << directory << " is not writable.";
        if (widget) {
            QMessageBox::critical(nullptr, tr("Unable to change log directory"),
                                  QString("Directory %1 is not writable").arg(directory));
        }
    }
  }
}
