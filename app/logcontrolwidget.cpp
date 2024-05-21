#include "logcontrolwidget.h"
#include "ui_logcontrolwidget.h"

#include <QtWidgets/qfiledialog.h>
#include <QtCore/qfileinfo.h>

#include <memory>

LogControlWidget::LogControlWidget(QWidget *parent): QWidget(parent), ui(std::make_unique<Ui::LogControlWidget>()) {
    ui->setupUi(this);
    QObject::connect(ui->logDirectoryButton, &QToolButton::clicked, this, &LogControlWidget::logDirectoryButtonClicked());
    QObject::connect(ui->recordButton, &QPushButton::clicked, [&](){emit setRecordingTriggered(!recording);});
}

LogControlWidget::~LogControlWidget() = default;

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