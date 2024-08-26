#include "state.h"
#include <QDir>
#include <QMessageBox>
#include <QTimer>
#include <QStorageInfo>

#include <chrono>

namespace wigglewatcher {
  State::State(): recording(false), log_directory(QDir::toNativeSeparators(QDir::currentPath())), bytes_available(-1), data_out_rate_Bps(-1), data_in_rate_Bps(-1), sample_rate_hz(-1.0) {}
}

StateObject::StateObject(QObject* parent, QWidget* widget_): QObject(parent), widget(widget_), timer(new QTimer(this)), bytes_written_since_last(0), bytes_read_since_last(0), samples_since_last(0) {
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
    if (state.recording) {
      const auto clicked = QMessageBox::question(widget, "Stop Recording?", "Press 'Ok' to stop recording.",
                                                 QMessageBox::StandardButton::Ok | QMessageBox::StandardButton::Cancel, 
                                                 QMessageBox::StandardButton::Cancel);
      if (clicked == QMessageBox::StandardButton::Ok) {
        state.recording = !state.recording;
      }
    }
    else {
      state.recording = !state.recording;
    }
    emit stateChanged(state);
}

auto StateObject::setLogDirectory(QString directory) noexcept -> void {
  if (directory != state.log_directory) {
    if (QFileInfo(directory).isWritable()) {
       state.log_directory = directory;
       storage_info.setPath(directory);
       state.bytes_available = storage_info.bytesAvailable();
       state.data_out_rate_Bps = -1;
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

auto StateObject::updateBytesRead(qint64 bytes) noexcept -> void {
  bytes_read_since_last += bytes;
}

auto StateObject::updateSampleCount(qint64 samples) noexcept -> void {
  samples_since_last += samples;
}

auto StateObject::handleTimerUpdate() noexcept -> void {
    state.data_in_rate_Bps = bytes_read_since_last / (timer->interval() / 1000.0);
    state.sample_rate_hz = static_cast<float>(samples_since_last) / (timer->interval() / 1000.0);

    if (storage_info.isValid()) {
      storage_info.refresh();
      state.bytes_available = storage_info.bytesAvailable();
    }

    state.data_out_rate_Bps = bytes_written_since_last / (timer->interval() / 1000.0);

    bytes_written_since_last = 0;
    bytes_read_since_last = 0;
    samples_since_last = 0;

    emit stateChanged(state);
}
