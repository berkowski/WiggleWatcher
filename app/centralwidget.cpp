#include "centralwidget.h"
#include "maggieplotwidget.h"
#include "logcontrolwidget.h"

#include <QLayout>
CentralWidget::CentralWidget(QWidget *parent): log_control_widget(new LogControlWidget), maggie_plot_widget(new MaggiePlotWidget) {
    auto layout = new QVBoxLayout();
    layout->addWidget(maggie_plot_widget, 1);
    layout->addWidget(log_control_widget, 0);
    setLayout(layout);

    QObject::connect(log_control_widget, &LogControlWidget::setLogDirectoryTriggered, this, &CentralWidget::setLogDirectoryTriggered);
    QObject::connect(log_control_widget, &LogControlWidget::setRecordingTriggered, this, &CentralWidget::setRecordingTriggered);
}

auto CentralWidget::updateState(wigglewatcher::State state) -> void {
    log_control_widget->updateState(std::move(state));
}

auto CentralWidget::addVectorMagnetometerData(const QString& name, const VectorMagnetometerData& data) -> void {

  maggie_plot_widget->addVectorMagnetometerData(name, data);

}
