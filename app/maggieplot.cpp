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
  : QObject(parent)
{
}

auto MaggiePlot::axisRect() const noexcept -> QCPAxisRect* {
  return axis_rect;
}

auto MaggiePlot::addVectorMagnetometerData(const QString& name, const VectorMagnetometerData& data) -> void {
}
auto MaggiePlot::onTimerUpdate() -> void {
}
