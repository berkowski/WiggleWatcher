#ifndef MAGGUI_MAGGIEPLOTWIDGET_H
#define MAGGUI_MAGGIEPLOTWIDGET_H

#include <QtWidgets/qwidget.h>
class QCustomPlot;
class QCPAxisRect;

class MaggiePlotWidget: public QWidget {
Q_OBJECT

public:
    explicit MaggiePlotWidget(QWidget* parent=nullptr);

private:
    QCustomPlot* plot;
    QList<QCPAxisRect*> axes;
};


#endif //MAGGUI_MAGGIEPLOTWIDGET_H
