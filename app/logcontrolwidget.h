#ifndef MAGGUI_LOGCONTROLWIDGET_H
#define MAGGUI_LOGCONTROLWIDGET_H

#include "state.h"

#include <QtWidgets/qwidget.h>

namespace Ui {
    class LogControlWidget;
}

class LogControlWidget: public QWidget {
Q_OBJECT

public:
    explicit LogControlWidget(QWidget* parent=nullptr);
    ~LogControlWidget() override;

signals:
    auto userChangedLogDirectory(QString) -> void;
    auto userSetRecordingEnabled(bool enabled) -> void;

public slots:
    auto updateState(maggui::State state) -> void;
  
private:
    std::unique_ptr<Ui::LogControlWidget> ui;
    bool recording = false;
			  
private slots:
    auto on_logDirectoryButton_clicked() -> void;
    auto on_recordButton_clicked() -> void;

};


#endif //MAGGUI_LOGCONTROLWIDGET_H
