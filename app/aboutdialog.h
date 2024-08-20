#ifndef WIGGLEWATCHER_ABOUT_H
#define WIGGLEWATCHER_ABOUT_H

#include <QDialog>

namespace Ui {
    class AboutDialog;
}

class AboutDialog: public QDialog {
Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent=nullptr);
    ~AboutDialog() override;

private:
    std::unique_ptr<Ui::AboutDialog> ui;
};
#endif
