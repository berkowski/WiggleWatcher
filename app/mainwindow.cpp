#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "centralwidget.h"

#include <QtCore/qdir.h>
#include <QtCore/qurl.h>
#include <QtGui/qdesktopservices.h>
#include <QtWidgets/qmessagebox.h>
#include <QtWidgets/qfiledialog.h>

#include <memory>

MainWindow::MainWindow(QWidget *parent,  Qt::WindowFlags flags): QMainWindow(parent, flags), ui(std::make_unique<Ui::MainWindow>())
{
    ui->setupUi(this);

    const auto central_widget = new CentralWidget;
    setCentralWidget(new CentralWidget);

    QObject::connect(central_widget, &CentralWidget::ogDirectory, this, &MainWindow::chooseLogDirectory);
    QObject::connect(central_widget, &CentralWidget::userSetRecordingEnabled, this, &MainWindow::userSetRecordingEnabled);

    QObject::connect(ui->actionAPS1540_Manual, &QAction::triggered, this, &MainWindow::showAps1540Manual);
    QObject::connect(ui->actionHMR2300_Manual, &QAction::triggered, this, &MainWindow::showHmr2300Manual);

}

MainWindow::~MainWindow() = default;

auto MainWindow::updateState(maggui::State state) -> void {
    qobject_cast<CentralWidget*>(centralWidget())->updateState(std::move(state));
}

auto MainWindow::addVectorMagnetometerData(const QString& name, const VectorMagnetometerData& data) -> void {

    qobject_cast<CentralWidget*>(centralWidget())->addVectorMagnetometerData(name, data);

}

auto MainWindow::showAps1540Manual() -> void
{

    const auto local = QDir::temp().filePath("APS1540_manual.pdf");
    if(!QFile::exists(local) && !QFile::copy(":/documents/APS1540_manual.pdf", local))
    {
        const auto message = QString("Unable to copy APS1540 manual to %1").arg(local);
        qWarning() << message;
        QMessageBox::warning(this, tr("Unable to open APS1540 manual"), message);
        return;
    }
    if(!QDesktopServices::openUrl(QUrl::fromLocalFile(local)))
    {
        qWarning() << "Unable to open " << local << " using default application";
    }
}

auto MainWindow::showHmr2300Manual() -> void
{
    const auto local = QDir::temp().filePath("SmartDigitalMagnetometerHMR2300_ds.pdf");
    if(!QFile::exists(local) && !QFile::copy(":/documents/SmartDigitalMagnetometerHMR2300_ds.pdf", local))
    {
        const auto message = QString("Unable to copy HMR2300 manual to %1").arg(local);
        qWarning() << message;
        QMessageBox::warning(this, tr("Unable to open HMR2300 manual"), message);
        return;
    }
    if(!QDesktopServices::openUrl(QUrl::fromLocalFile(local)))
    {
        qWarning() << "Unable to open " << local << " using default application";
    }
}

auto MainWindow::chooseLogDirectory() -> void {
    const auto dir = QFileDialog::getExistingDirectory(this, tr("Save logs to..."), log_directory, QFileDialog::ShowDirsOnly);
    if (!dir.isNull() && dir != log_directory) {
        log_directory = dir;
        emit logDirectoryChanged(dir);
    }
}
