#ifndef MAGGUI_LOGCONTROLWIDGET_H
#define MAGGUI_LOGCONTROLWIDGET_H

#include "state.h"

#include <QWidget>

namespace Ui {
    class LogControlWidget;
}

class LogControlWidget: public QWidget {
Q_OBJECT

public:
    explicit LogControlWidget(QWidget* parent=nullptr);
    ~LogControlWidget() override;

signals:
    auto setLogDirectoryTriggered() -> void;
    auto setRecordingTriggered() -> void;

public slots:
    auto updateState(maggui::State state) -> void;
  
private:
    std::unique_ptr<Ui::LogControlWidget> ui;
    bool recording = false;
};


#endif //MAGGUI_LOGCONTROLWIDGET_H
