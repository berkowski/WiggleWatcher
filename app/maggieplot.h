#ifndef WIGGLEWATCHER_MAGGIEPLOT_H
#define WIGGLEWATCHER_MAGGIEPLOT_H

#include <core/vectormagnetometerdata.h>

#include <QObject>

class QCPAxisRect;
class QCustomPlot;

class MaggiePlot: public QObject {
  Q_OBJECT

 public:
   explicit MaggiePlot(QCustomPlot* plot, QObject* parent = nullptr);
   auto axisRect() const noexcept -> QCPAxisRect*;

 public slots:
   auto addVectorMagnetometerData(const QString& name, const VectorMagnetometerData& data) -> void;
   auto onTimerUpdate() -> void;

 private:
   QCPAxisRect* axis_rect;

};
#endif
