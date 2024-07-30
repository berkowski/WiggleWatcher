#include "state.h"
#include <QDir>
#include <QMessageBox>
#include <QTimer>
#include <QStorageInfo>

#include <chrono>

namespace maggui {
  State::State(): recording(false), log_directory(QDir::toNativeSeparators(QDir::currentPath())), bytes_available(-1), data_rate_Bps(-1) {}
}

StateObject::StateObject(QObject* parent, QWidget* widget_): QObject(parent), widget(widget_), timer(new QTimer(this)), bytes_written_since_last(0) {
  timer->setInterval(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::seconds(10)));
  QObject::connect(timer, &QTimer::timeout, this, &StateObject::handleTimerUpdate);
  timer->start();
};

auto StateObject::setRecordingEnabled(bool enabled) noexcept -> void {
  if (enabled != state.recording) {
    state.recording = enabled;
    emit stateChanged(state);
  }
}

auto StateObject::toggleRecordingEnabled() noexcept -> void
{
    state.recording = !state.recording;
    emit stateChanged(state);
}

auto StateObject::setLogDirectory(QString directory) noexcept -> void {
  if (directory != state.log_directory) {
    if (QFileInfo(directory).isWritable()) {
       state.log_directory = directory;
       storage_info.setPath(directory);
       state.bytes_available = storage_info.bytesAvailable();
       state.data_rate_Bps = -1;
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

auto StateObject::updateBytesWritten(qint64 bytes) noexcept -> void {
  bytes_written_since_last += bytes;
}

auto StateObject::handleTimerUpdate() noexcept -> void {
    if (!storage_info.isValid()) {
      return;
    }
    storage_info.refresh();

    state.bytes_available = storage_info.bytesAvailable();
    state.data_rate_Bps = bytes_written_since_last / (timer->interval() / 1000.0);

    bytes_written_since_last = 0;
    emit stateChanged(state);
}
