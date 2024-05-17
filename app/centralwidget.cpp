#include "centralwidget.h"
#include "maggieplotwidget.h"
#include "logcontrolwidget.h"

#include <QtWidgets/qlayout.h>
CentralWidget::CentralWidget(QWidget *parent): log_control_widget(new LogControlWidget), maggie_plot_widget(new MaggiePlotWidget) {
    auto layout = new QVBoxLayout();
    layout->addWidget(maggie_plot_widget, 1);
    layout->addWidget(log_control_widget, 0);
    setLayout(layout);

    QObject::connect(log_control_widget, &LogControlWidget::userChangedLogDirectory, this, &CentralWidget::userChangedLogDirectory);
    QObject::connect(log_control_widget, &LogControlWidget::userSetRecordingEnabled, this, &CentralWidget::userSetRecordingEnabled);
}

auto CentralWidget::updateState(maggui::State state) -> void {
    log_control_widget->updateState(std::move(state));
}
