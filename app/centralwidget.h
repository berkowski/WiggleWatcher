#ifndef MAGGUI_CENTRALWIDGET_H
#define MAGGUI_CENTRALWIDGET_H

#include "state.h"
#include <core/vectormagnetometerdata.h>

#include <QtWidgets/qwidget.h>

class LogControlWidget;
class MaggiePlotWidget;
class VectorMagnetometerData;

class CentralWidget: public QWidget {
Q_OBJECT
public:
    explicit CentralWidget(QWidget *parent=nullptr);

signals:
    auto changeLogDirectoryTriggered() -> void;
    auto recordingEnabledTriggered(bool enabled) -> void;

public slots:
    auto updateState(maggui::State state) -> void;
    auto addVectorMagnetometerData(const QString& name, const VectorMagnetometerData& data) -> void;

private:
    LogControlWidget* log_control_widget;
    MaggiePlotWidget* maggie_plot_widget;
};


#endif //MAGGUI_CENTRALWIDGET_H
