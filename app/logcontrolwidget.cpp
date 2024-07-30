#include "logcontrolwidget.h"
#include "ui_logcontrolwidget.h"

#include <memory>

LogControlWidget::LogControlWidget(QWidget *parent): QWidget(parent), ui(std::make_unique<Ui::LogControlWidget>()) {
    ui->setupUi(this);
    QObject::connect(ui->logDirectoryButton, &QToolButton::clicked, this, &LogControlWidget::setLogDirectoryTriggered);
    QObject::connect(ui->recordButton, &QPushButton::clicked, [&](){emit setRecordingTriggered();});
}

LogControlWidget::~LogControlWidget() = default;

auto LogControlWidget::updateState(maggui::State state) -> void {

  auto locale = QLocale();
  locale.setNumberOptions(QLocale::DefaultNumberOptions);

  if (state.log_directory != ui->logDirectory->text()) {
    ui->logDirectory->setText(state.log_directory);
  }
  if (state.bytes_available < 0) {
      ui->freeSpace->setText("unknown");
  }
  else {
      if (state.bytes_available < 1000) {
          ui->freeSpace->setText(QStringLiteral("%1 B").arg(state.bytes_available));
      } else if (state.bytes_available < 1000 * 1000) {
          ui->freeSpace->setText(QStringLiteral("%1 kB").arg(locale.toString(state.bytes_available / 1000.0)));
      } else if (state.bytes_available < 1000 * 1000 * 1000) {
          ui->freeSpace->setText(QStringLiteral("%1 MB").arg(locale.toString(state.bytes_available / (1000.0 * 1000.0))));
      } else {
          ui->freeSpace->setText(QStringLiteral("%1 GB").arg(locale.toString(state.bytes_available / (1000.0 * 1000.0 * 1000.0))));
      }
  }

  if (state.data_rate_Bps <= 0) {
      ui->dataRate->setText(QStringLiteral("--"));
      ui->availableRecordingTime->setText(QStringLiteral("--"));
  }
  else {
      if (state.data_rate_Bps < 1000) {
          ui->dataRate->setText(QStringLiteral("%1 B/s").arg(locale.toString(state.data_rate_Bps)));
      }
      else {
          ui->dataRate->setText(QStringLiteral("%1 kB/s").arg(locale.toString(state.data_rate_Bps / 1000.0)));
      }

      // in seconds
      const auto recording_time_remaining = state.bytes_available / state.data_rate_Bps;

      const auto days = recording_time_remaining / (24 * 3600);
      const auto hours = (recording_time_remaining - (days * 24 * 3600)) / 3600;
      const auto minutes = (recording_time_remaining - (days * 24 * 3600) - (hours * 3600)) / 60;
      const auto seconds = recording_time_remaining - (days * 24 * 3600) - (hours * 3600) - (minutes * 60);

      if (days > 10) {
          ui->availableRecordingTime->setText(QStringLiteral("> 10 days"));
      }
      else if (days > 0) {
          ui->availableRecordingTime->setText(QStringLiteral("%1 day%2 %3:%4:%5")
            .arg(days).arg(days > 1 ? QStringLiteral("s") : QStringLiteral(""))
            .arg(hours, 2, 10, QChar('0'))
            .arg(minutes, 2, 10, QChar('0'))
            .arg(seconds, 2, 10, QChar('0')));
      }
      else {
          ui->availableRecordingTime->setText(QStringLiteral("%1:%2:%3")
            .arg(hours, 2, 10, QChar('0'))
            .arg(minutes, 2, 10, QChar('0'))
            .arg(seconds, 2, 10, QChar('0')));
      }
  }

  if (state.recording) {
      ui->recordButton->setText("&Stop\nRecording");
  }
  else {
    ui->recordButton->setText("&Record");
  }
}
