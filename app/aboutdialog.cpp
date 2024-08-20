#include "aboutdialog.h"
#include "ui_aboutdialog.h"

#include <core/global.h>

#include <memory>

AboutDialog::AboutDialog(QWidget *parent): QDialog(parent), ui(std::make_unique<Ui::AboutDialog>()) {
    ui->setupUi(this);
    ui->compile_value->setText(__DATE__);
    ui->version_value->setText(wigglewatcher::WIGGLEWATCHER_VERSION);
}

AboutDialog::~AboutDialog() = default;
