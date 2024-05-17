#include "logcontrolwidget.h"
#include "ui_logcontrolwidget.h"

#include <QtWidgets/qfiledialog.h>
#include <QtCore/qfileinfo.h>

#include <memory>

LogControlWidget::LogControlWidget(QWidget *parent): QWidget(parent), ui(std::make_unique<Ui::LogControlWidget>()) {
    ui->setupUi(this);
}

LogControlWidget::~LogControlWidget() = default;

auto LogControlWidget::on_logDirectoryButton_clicked() -> void {
    const auto current_directory = ui->logDirectory->text();
    const auto dir = QFileDialog::getExistingDirectory(this, tr("Save logs to..."), current_directory, QFileDialog::ShowDirsOnly);
    if (!dir.isNull() && dir != current_directory) {
        emit userChangedLogDirectory(dir);
    }
}

auto LogControlWidget::updateState(maggui::State state) -> void {

  if (state.log_directory != ui->logDirectory->text()) {
    ui->logDirectory->setText(state.log_directory);
  }

  if (state.recording != recording) {
    if (state.recording) {
      ui->recordButton->setText("&Stop\nRecording");
    }
    else {
      ui->recordButton->setText("&Record");
    }
    recording = state.recording;
  }
}

auto LogControlWidget::on_recordButton_clicked() -> void
{
  emit userSetRecordingEnabled(!recording);
}
