#include "maggieplotwidget.h"

#include <qcustomplot.h>
#include <QTimer>
#include <QLayout>

enum MaggiePlotIndex {
    X = 0,
    Y,
    Z,
    TOTAL,
    NUM_PLOTS,
};

auto toString(MaggiePlotIndex index) -> QString {
    switch (index) {
    case MaggiePlotIndex::X:
        return QString{"X"};
    case MaggiePlotIndex::Y:
        return QString{"Y"};
    case MaggiePlotIndex::Z:
        return QString{"Z"};
    case MaggiePlotIndex::TOTAL:
        return QString{"Total"};
    default:
        Q_UNREACHABLE();
    }
}

MaggiePlotWidget::MaggiePlotWidget(QWidget *parent)
  : QWidget(parent)
  , plot(new QCustomPlot(this))
  , timer(new QTimer(this))
{
    const auto datetime_now = QDateTime::currentDateTimeUtc();
    plot->plotLayout()->clear();
    plot->plotLayout()->setWrap(0);
    plot->plotLayout()->setFillOrder(QCPLayoutGrid::FillOrder::foRowsFirst);
    plot->setAutoAddPlottableToLegend(false);

    axis_rects.resize(MaggiePlotIndex::NUM_PLOTS);

    // x-axis datetime ticker
    auto datetime_ticker = QSharedPointer<QCPAxisTickerDateTime>{new QCPAxisTickerDateTime};
    datetime_ticker->setDateTimeFormat("MMM dd\nHH:mm:ss");
    datetime_ticker->setTickStepStrategy(QCPAxisTicker::tssReadability);

    // create plot axes
    auto enum_index = 0;
    std::for_each(std::begin(axis_rects), std::end(axis_rects), [&](auto& it) {
        // make a new axis item
        it = new QCPAxisRect(plot);

        // hide all axis except the left y for now, but set the x-axis ticker and enable
        // grid lines.  Disable vertical dragging and zooming.
        auto axes = it->axes();
        std::for_each(std::begin(axes), std::end(axes), [&](auto& axis) {
          if (axis->axisType() == QCPAxis::AxisType::atRight || axis->axisType() == QCPAxis::AxisType::atTop)  {
            axis->setVisible(false);
          }
          else if(axis->axisType() == QCPAxis::AxisType::atBottom) {
            axis->setTicker(datetime_ticker);
            axis->setTickLabels(false);
            axis->setTicks(false);

            it->setRangeZoomAxes(axis, nullptr);
            it->setRangeDragAxes(axis, nullptr);
          }
          else if(axis->axisType() == QCPAxis::AxisType::atLeft) {
              axis->setLabel(QString("%1 (nT)").arg(toString(static_cast<MaggiePlotIndex>(enum_index))));
              axis->ticker()->setTickStepStrategy(QCPAxisTicker::TickStepStrategy::tssReadability);
          }
        });
        auto axes_layout = it->insetLayout();
        auto legend = new QCPLegend{};
        legend->setBrush(QBrush{Qt::black, Qt::SolidPattern});
        legend->setFillOrder(QCPLayoutGrid::FillOrder::foColumnsFirst);
        axes_layout->addElement(static_cast<QCPLayoutElement*>(legend), Qt::AlignLeft | Qt::AlignTop);

        plot->plotLayout()->addElement(it);
        enum_index++;
    });

    // link scrolling of x-axes
    std::for_each(std::begin(axis_rects), std::prev(std::end(axis_rects)), [&](auto& rect) {
      const auto primary_axis = axis_rects.back()->axis(QCPAxis::AxisType::atBottom);
      const auto linked_axis = rect->axis(QCPAxis::AxisType::atBottom);
      
      QObject::connect(primary_axis, qOverload<const QCPRange&>(&QCPAxis::rangeChanged), linked_axis, qOverload<const QCPRange&>(&QCPAxis::setRange));
      QObject::connect(linked_axis, qOverload<const QCPRange&>(&QCPAxis::rangeChanged), primary_axis, qOverload<const QCPRange&>(&QCPAxis::setRange));
    });

    // setup time axis
    time_axis = axis_rects.back()->axis(QCPAxis::AxisType::atBottom);
    time_axis->setRange(static_cast<double>((datetime_now - std::chrono::duration_cast<std::chrono::milliseconds>(MAXIMUM_PLOT_HISTORY)).toMSecsSinceEpoch())/1000.0, static_cast<double>(datetime_now.toMSecsSinceEpoch())/1000.0);
    time_axis->setTickLabels(true);
    time_axis->setTicks(true);

    // user interactions
    // plot->setInteractions(QCP::iRangeZoom | QCP::iRangeDrag);
    plot->setInteractions(QCP::iNone);

    // setup plot timer
    timer->setInterval(std::chrono::duration_cast<std::chrono::milliseconds>(PLOT_UPDATE_INTERVAL).count());
    QObject::connect(timer, &QTimer::timeout, this, &MaggiePlotWidget::onTimerUpdate);
    timer->start();
    
    auto layout = new QVBoxLayout();
    layout->addWidget(plot);
    setLayout(layout);
}

auto MaggiePlotWidget::onTimerUpdate() -> void {
  const auto now = static_cast<double>(QDateTime::currentDateTimeUtc().toMSecsSinceEpoch()) / 1000.0;
  const auto oldest = now - static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(MAXIMUM_PLOT_HISTORY).count())/1000.0;

  // cull old data
  std::for_each(std::begin(data), std::end(data), [&](auto& graphs) {
    std::for_each(std::begin(graphs), std::end(graphs), [&](auto& graph) {
      graph->data()->removeBefore(oldest);
    });
  });

  // shift axes
  time_axis->setRange(oldest, now);

  // rescale vertical axis
  std::for_each(std::begin(axis_rects), std::end(axis_rects), [&](auto& rect) {
    auto y_axis = rect->axis(QCPAxis::AxisType::atLeft); 
    y_axis->rescale(true);
    const auto range = y_axis->range();
    if (range.size() < 10) {
      y_axis->setRange(range.center() - 5, range.center() + 5);
    }
  });
      
  plot->replot();
  
}

auto MaggiePlotWidget::addVectorMagnetometerData(const QString& name, const VectorMagnetometerData& data_) -> void {

  auto& maggie_graphs= data[name];
  
  if (maggie_graphs.isEmpty()) {
    std::for_each(std::begin(axis_rects), std::end(axis_rects), [&](auto& it) {
      const auto x_axis = it->axis(QCPAxis::AxisType::atBottom);
      const auto y_axis = it->axis(QCPAxis::AxisType::atLeft);
      
      auto graph = plot->addGraph(x_axis, y_axis);
      graph->setName(name);

      // add to legend
      auto legend = dynamic_cast<QCPLegend*>(it->insetLayout()->elementAt(0));
      Q_ASSERT(legend);
      graph->addToLegend(legend);
      maggie_graphs.push_back(graph);
    });
    assert(maggie_graphs.size() == MaggiePlotIndex::NUM_PLOTS);
  }

  const auto timestamp = static_cast<double>(data_.timestamp.toMSecsSinceEpoch())/1000.0;
  const auto total = qSqrt(data_.x * data_.x + data_.y * data_.y + data_.z * data_.z);
  maggie_graphs.at(MaggiePlotIndex::X)->addData(timestamp, data_.x);
  maggie_graphs.at(MaggiePlotIndex::Y)->addData(timestamp, data_.y);
  maggie_graphs.at(MaggiePlotIndex::Z)->addData(timestamp, data_.z);
  maggie_graphs.at(MaggiePlotIndex::TOTAL)->addData(timestamp, total);

  // don't do this here, we'll replot anyway on the next timer tic
  //plot->replot();
  
}
