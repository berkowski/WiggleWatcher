#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "centralwidget.h"
#include <memory>

MainWindow::MainWindow(QWidget *parent,  Qt::WindowFlags flags): QMainWindow(parent, flags), ui(std::make_unique<Ui::MainWindow>())
{
    ui->setupUi(this);

    const auto central_widget = new CentralWidget;
    setCentralWidget(new CentralWidget);

    QObject::connect(central_widget, &CentralWidget::userChangedLogDirectory, this, &MainWindow::userChangedLogDirectory);
    QObject::connect(central_widget, &CentralWidget::userSetRecordingEnabled, this, &MainWindow::userSetRecordingEnabled);
}

MainWindow::~MainWindow() = default;

auto MainWindow::updateState(maggui::State state) -> void {
    qobject_cast<CentralWidget*>(centralWidget())->updateState(std::move(state));
}

auto MainWindow::addVectorMagnetometerData(const QString& name, const VectorMagnetometerData& data) -> void {

    qobject_cast<CentralWidget*>(centralWidget())->addVectorMagnetometerData(name, data);

}
