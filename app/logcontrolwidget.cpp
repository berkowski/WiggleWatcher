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

  if (state.log_directory != ui->logDirectory->text()) {
    ui->logDirectory->setText(state.log_directory);
  }

  if (state.recording) {
      ui->recordButton->setText("&Stop\nRecording");
  }
  else {
    ui->recordButton->setText("&Record");
  }
}