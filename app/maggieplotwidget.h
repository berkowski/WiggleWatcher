#ifndef WIGGLEWATCHER_MAGGIEPLOTWIDGET_H
#define WIGGLEWATCHER_MAGGIEPLOTWIDGET_H

#include <core/vectormagnetometerdata.h>
#include <QWidget>

class QCustomPlot;
class QCPAxisRect;
class QCPAxis;
class QCPGraph;
class QCPRange;

class QTimer;

class MaggiePlotWidget: public QWidget {
Q_OBJECT

public:
    static auto constexpr MAXIMUM_PLOT_HISTORY = std::chrono::minutes {5};
    static auto constexpr PLOT_UPDATE_INTERVAL = std::chrono::milliseconds{2 * 16};

    explicit MaggiePlotWidget(QWidget* parent=nullptr);

public slots:
    auto addVectorMagnetometerData(const QString& name, const VectorMagnetometerData& data) -> void;
												   
private slots:
    auto onTimerUpdate() -> void;
  
private:
    QCustomPlot* plot;
    QList<QCPAxisRect*> axis_rects;
    QCPAxis* time_axis;
    QTimer* timer;
    QMap<QString, QList<QCPGraph*>> data;
};


#endif //WIGGLEWATCHER_MAGGIEPLOTWIDGET_H
