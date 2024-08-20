#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "centralwidget.h"

#include <QDir>
#include <QUrl>
#include <QDesktopServices>
#include <QMessageBox>
#include <QFileDialog>

#include <memory>

MainWindow::MainWindow(QWidget *parent,  Qt::WindowFlags flags): QMainWindow(parent, flags), ui(std::make_unique<Ui::MainWindow>())
{
    ui->setupUi(this);

    auto central_widget = qobject_cast<CentralWidget*>(centralWidget());
    QObject::connect(central_widget, &CentralWidget::setLogDirectoryTriggered, this,
                     &MainWindow::onSetLogDirectoryTriggered);

    QObject::connect(central_widget, &CentralWidget::setRecordingTriggered, this,
                     &MainWindow::setRecordingTriggered);

    QObject::connect(ui->actionAPS1540_Manual, &QAction::triggered, this, &MainWindow::showAps1540Manual);
    QObject::connect(ui->actionHMR2300_Manual, &QAction::triggered, this, &MainWindow::showHmr2300Manual);
    QObject::connect(ui->actionE_xit, &QAction::triggered, this, &MainWindow::close);

    // Unused at the moment as we do not support run-time reqconfiguration
    ui->action_Load_Settings->setVisible(false);
    ui->action_Save_Settings->setVisible(false);
}

MainWindow::~MainWindow() = default;

auto MainWindow::updateState(wigglewatcher::State state) -> void {
    log_directory = state.log_directory;
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
        const auto message = QStringLiteral("Unable to open APS1540 manual copied to %1 using default application.").arg(local);
        QMessageBox::warning(this, tr("Unable to open APS1540 manual"), message);
        qWarning() << message;
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
        const auto message = QStringLiteral("Unable to open HMR2300 manual copied to %1 using default application.").arg(local);
        QMessageBox::warning(this, tr("Unable to open HMR2300 manual"), message);
        qWarning() << message;
    }
}

auto MainWindow::onSetLogDirectoryTriggered() -> void {
    const auto dir = QFileDialog::getExistingDirectory(this, tr("Save logs to..."), log_directory, QFileDialog::ShowDirsOnly);
    if (!dir.isNull()) {
        emit logDirectoryChanged(QDir::toNativeSeparators(dir));
    }
}
