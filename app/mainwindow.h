#ifndef MAGGUI_MAINWINDOW_H
#define MAGGUI_MAINWINDOW_H

#include <QMainWindow>
#include <core/vectormagnetometerdata.h>

#include "state.h"

namespace Ui {
class MainWindow;
}

class MainWindow: public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent=nullptr, Qt::WindowFlags=Qt::WindowFlags());
    ~MainWindow() override;

signals:
    auto setRecordingTriggered() -> void;
    auto logDirectoryChanged(const QString &directory) -> void;

public slots:
    auto updateState(maggui::State state) -> void;
    auto addVectorMagnetometerData(const QString& name, const VectorMagnetometerData& data) -> void;
    auto onSetLogDirectoryTriggered() -> void;

private slots:
    auto showAps1540Manual() -> void;
    auto showHmr2300Manual() -> void;

private:
    std::unique_ptr<Ui::MainWindow> ui;
    QString log_directory;
};

#endif //MAGGUI_MAINWINDOW_H
