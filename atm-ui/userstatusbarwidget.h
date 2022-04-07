#ifndef USERSTATUSBARWIDGET_H
#define USERSTATUSBARWIDGET_H

#include <QWidget>

namespace Ui {
    class UserStatusBarWidget;
}

class UserStatusBarWidget : public QWidget {
    Q_OBJECT

    public:
        explicit UserStatusBarWidget(QWidget *parent = nullptr);
        ~UserStatusBarWidget();

    private:
        Ui::UserStatusBarWidget *ui;
};

#endif // USERSTATUSBARWIDGET_H
