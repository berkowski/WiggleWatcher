#ifndef MAGGUI_LOGCONTROLWIDGET_H
#define MAGGUI_LOGCONTROLWIDGET_H

#include <QtWidgets/qwidget.h>

namespace Ui {
    class LogControlWidget;
}

class LogControlWidget: public QWidget {
Q_OBJECT

public:
    explicit LogControlWidget(QWidget* parent=nullptr);

signals:
    auto logDirectoryChanged(QString) -> void;

private slots:
    auto on_LogDirectoryButton_clicked() -> void;

private:
    std::unique_ptr<Ui::LogControlWidget> ui;
};


#endif //MAGGUI_LOGCONTROLWIDGET_H
