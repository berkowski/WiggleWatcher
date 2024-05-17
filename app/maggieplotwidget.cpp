#include "maggieplotwidget.h"

#include <qcustomplot.h>
#include <QtWidgets/qlayout.h>

enum MaggiePlotIndex {
    X = 0,
    Y,
    Z,
    TOTAL,
    NUM_PLOTS,
};

MaggiePlotWidget::MaggiePlotWidget(QWidget *parent): QWidget(parent), plot(new QCustomPlot(this))
{
    plot->plotLayout()->clear();
    plot->plotLayout()->setWrap(0);
    plot->plotLayout()->setFillOrder(QCPLayoutGrid::FillOrder::foRowsFirst);
    axes.resize(MaggiePlotIndex::NUM_PLOTS);
    std::for_each(std::begin(axes), std::end(axes), [&](auto& it) {
        auto rect = new QCPAxisRect(plot);
        plot->plotLayout()->addElement(rect);
    });

    auto layout = new QVBoxLayout();
    layout->addWidget(plot);
    setLayout(layout);
}