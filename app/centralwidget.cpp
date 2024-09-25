#include "centralwidget.h"
#include "maggieplot.h"
#include "logcontrolwidget.h"

#include <core/sensorlogger.h>
#include <core/magnetometerlogger.h>

#include <qcustomplot.h>

#include <QLayout>

CentralWidget::CentralWidget(QWidget *parent): log_control_widget(new LogControlWidget), plot(new QCustomPlot(this)), margin_group(new QCPMarginGroup(plot)), datetime_ticker(new QCPAxisTickerDateTime), timer(new QTimer(this)) {

  plot->plotLayout()->clear();
  plot->plotLayout()->setWrap(0);
  plot->plotLayout()->setFillOrder(QCPLayoutGrid::FillOrder::foRowsFirst);

  // x-axis datetime ticker
  //auto datetime_ticker = QSharedPointer<QCPAxisTickerDateTime>{new QCPAxisTickerDateTime};
  datetime_ticker->setDateTimeFormat("MMM dd\nHH:mm:ss");
  datetime_ticker->setTickStepStrategy(QCPAxisTicker::tssReadability);

  // margin group to align left and right axes
  //auto margin_group = new QCPMarginGroup(plot);



  auto layout = new QVBoxLayout();
  layout->addWidget(plot, 1);
  layout->addWidget(log_control_widget, 0);
  setLayout(layout);

  QObject::connect(log_control_widget, &LogControlWidget::setLogDirectoryTriggered, this, &CentralWidget::setLogDirectoryTriggered);
  QObject::connect(log_control_widget, &LogControlWidget::setRecordingTriggered, this, &CentralWidget::setRecordingTriggered);

  timer->setInterval(std::chrono::duration_cast<std::chrono::milliseconds>(PLOT_UPDATE_INTERVAL).count());
  QObject::connect(timer, &QTimer::timeout, this, &CentralWidget::onTimerUpdate);

  timer->start();

    
}

auto CentralWidget::updateState(wigglewatcher::State state) -> void {
  log_control_widget->updateState(std::move(state));
}

auto CentralWidget::addLogger(SensorLogger* logger) -> void {

  if(!logger) {
    return;
  }

  else if(auto maggie_logger = qobject_cast<MagnetometerLogger*>(logger); maggie_logger) {
    auto graph = new MaggiePlot(plot, this);
    auto rect = graph->axisRect();
    auto bottom_axis = rect->axis(QCPAxis::AxisType::atBottom);
    bottom_axis->setTicker(datetime_ticker);

    QObject::connect(maggie_logger, &MagnetometerLogger::valueChanged, graph, [=](const auto& name, const auto& kind, const auto& data) {
      Q_UNUSED(kind);
      graph->addVectorMagnetometerData(name, qvariant_cast<VectorMagnetometerData>(data));
    }, Qt::QueuedConnection);
  }
  
  //maggie_plot_widget->addVectorMagnetometerData(name, data);

}

auto CentralWidget::onTimerUpdate() -> void {
  const auto now = static_cast<double>(QDateTime::currentDateTimeUtc().toMSecsSinceEpoch()) / 1000.0;
  const auto oldest = now - static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(MAXIMUM_PLOT_HISTORY).count())/1000.0;

  auto rects = plot->axisRects();
  std::for_each(std::begin(rects), std::end(rects), [&](auto& rect) {
    auto graphs = rect->graphs();
    std::for_each(std::begin(graphs), std::end(graphs), [&](auto& graph) {
      graph->data()->removeBefore(oldest);
      auto value_axis = graph->valueAxis();
      value_axis->rescale(true);
      const auto range = value_axis->range();
      if (range.size() < 50) {
        value_axis->setRange(range.center() - 25, range.center() + 25);
      }

    });
    
    auto bottom_axis = rect->axis(QCPAxis::AxisType::atBottom, 0);
    bottom_axis->setRange(oldest, now);

  });
  plot->replot();
}
