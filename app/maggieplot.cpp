#include "maggieplot.h"

#include <qcustomplot.h>

enum MaggiePlotAxis {
  X = 0,
  Y,
  Z,
  TOTAL,
  NUM_AXES,
};

auto toString(MaggiePlotAxis index) -> QString {
  switch (index) {
  case MaggiePlotAxis::X:
    return QString{"X"};
  case MaggiePlotAxis::Y:
    return QString{"Y"};
  case MaggiePlotAxis::Z:
    return QString{"Z"};
  case MaggiePlotAxis::TOTAL:
    return QString{"Total"};
  default:
    Q_UNREACHABLE();
  }
}

MaggiePlot::MaggiePlot(QCustomPlot* plot, QObject *parent)
  : QObject(parent), axis_rect(new QCPAxisRect(plot, false))
{
    const auto x_axis = axis_rect->addAxis(QCPAxis::AxisType::atBottom);

    qInfo() << plot->axisRect();
    for(auto n = 0; n < MaggiePlotAxis::NUM_AXES; n++) {
      const auto y_axis = axis_rect->addAxis(QCPAxis::AxisType::atLeft);
      auto graph = plot->addGraph(x_axis, y_axis);
      graph->setName(toString(static_cast<MaggiePlotAxis>(n)));
      graphs.append(graph);
    }

    plot->plotLayout()->addElement(axis_rect);
}

auto MaggiePlot::axisRect() const noexcept -> QCPAxisRect* {
  return axis_rect;
}

auto MaggiePlot::addVectorMagnetometerData(const QString& name, const VectorMagnetometerData& data) -> void {

  const auto timestamp = static_cast<double>(data.timestamp.toMSecsSinceEpoch())/1000.0;
  const auto total = qSqrt(data.x * data.x + data.y * data.y + data.z * data.z);
  graphs.at(MaggiePlotAxis::X)->addData(timestamp, data.x);
  graphs.at(MaggiePlotAxis::Y)->addData(timestamp, data.y);
  graphs.at(MaggiePlotAxis::Z)->addData(timestamp, data.z);
  graphs.at(MaggiePlotAxis::TOTAL)->addData(timestamp, total);
}

auto MaggiePlot::onTimerUpdate() -> void {
}
