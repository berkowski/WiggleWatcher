#ifndef WIGGLEWATCHER_CENTRALWIDGET_H
#define WIGGLEWATCHER_CENTRALWIDGET_H

#include "state.h"
#include <core/vectormagnetometerdata.h>

#include <QWidget>

class LogControlWidget;
class MaggiePlotWidget;
class VectorMagnetometerData;

class CentralWidget: public QWidget {
Q_OBJECT
public:
    explicit CentralWidget(QWidget *parent=nullptr);

signals:
    auto setLogDirectoryTriggered() -> void;
    auto setRecordingTriggered() -> void;

public slots:
    auto updateState(wigglewatcher::State state) -> void;
    auto addVectorMagnetometerData(const QString& name, const VectorMagnetometerData& data) -> void;

private:
    LogControlWidget* log_control_widget;
    MaggiePlotWidget* maggie_plot_widget;
};


#endif //WIGGLEWATCHER_CENTRALWIDGET_H
