#ifndef MAGGUI_MAINWINDOW_H
#define MAGGUI_MAINWINDOW_H

#include <QtWidgets/qmainwindow.h>
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
    auto userChangedLogDirectory(QString) -> void;
    auto userSetRecordingEnabled(bool enabled) -> void;

public slots:
    auto updateState(maggui::State state) -> void;
    auto addVectorMagnetometerData(const QString& name, const VectorMagnetometerData& data) -> void;


private:
    std::unique_ptr<Ui::MainWindow> ui;
};

#endif //MAGGUI_MAINWINDOW_H
