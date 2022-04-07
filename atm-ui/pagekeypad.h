#ifndef PAGEKEYPAD_H
#define PAGEKEYPAD_H

#include "userstatusbarwidget.h"

#include <QWidget>

namespace Ui {
    class PageKeypad;
}

class PageKeypad : public QWidget {
    Q_OBJECT

    public:

        enum Action {
            Withdraw,
            Deposit
        };

        explicit PageKeypad(PageKeypad::Action action, QWidget *parent = nullptr);
        ~PageKeypad();

    private slots:
        void onKeypadButtonPress();

    private:
        Ui::PageKeypad *ui;
        UserStatusBarWidget *barWidget;
};

#endif // PAGEKEYPAD_H
