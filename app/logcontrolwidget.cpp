#include "logcontrolwidget.h"
#include "ui_logcontrolwidget.h"

#include <QtWidgets/qfiledialog.h>

#include <memory>

LogControlWidget::LogControlWidget(QWidget *parent): QWidget(parent), ui(std::make_unique<Ui::LogControlWidget>()) {
    ui->setupUi(this);
    QMetaObject::connectSlotsByName(this);
}

auto LogControlWidget::on_LogDirectoryButton_clicked() -> void {
    const auto dir = QFileDialog::getExistingDirectory(this, tr("Use log directory"), "", QFileDialog::ShowDirsOnly);
    if (!dir.isNull() && dir != ui->logDirectory->text()) {
        ui->logDirectory->setText(dir);
        emit logDirectoryChanged(dir);
    }
}
