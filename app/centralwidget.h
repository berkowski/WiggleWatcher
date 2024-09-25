#ifndef WIGGLEWATCHER_CENTRALWIDGET_H
#define WIGGLEWATCHER_CENTRALWIDGET_H

#include "state.h"
#include <core/vectormagnetometerdata.h>

#include <QWidget>

class LogControlWidget;
class MaggiePlotWidget;
class VectorMagnetometerData;
class SensorLogger;

class QCustomPlot;
class QCPAxisTickerDateTime;
class QCPMarginGroup;

class QTimer;

class CentralWidget: public QWidget {
Q_OBJECT
public:
    static auto constexpr MAXIMUM_PLOT_HISTORY = std::chrono::minutes {5};
    static auto constexpr PLOT_UPDATE_INTERVAL = std::chrono::milliseconds{2 * 16};

    explicit CentralWidget(QWidget *parent=nullptr);

signals:
    auto setLogDirectoryTriggered() -> void;
    auto setRecordingTriggered() -> void;

public slots:
    auto updateState(wigglewatcher::State state) -> void;
    auto addLogger(SensorLogger* logger) -> void;

private slots:
  auto onTimerUpdate() -> void;
  
private:
    LogControlWidget* log_control_widget;
    QCustomPlot* plot;
  QCPMarginGroup* margin_group;
  QSharedPointer<QCPAxisTickerDateTime> datetime_ticker;
  QTimer* timer;
};


#endif //WIGGLEWATCHER_CENTRALWIDGET_H
