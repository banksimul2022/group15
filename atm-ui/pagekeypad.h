#ifndef PAGEKEYPAD_H
#define PAGEKEYPAD_H

#include "pagewithuserbar.h"
#include "userstatusbarwidget.h"

#include <QWidget>

namespace Ui {
    class PageKeypad;
}

class PageKeypad : public PageWithUserBar {
    Q_OBJECT

    public:

        enum Action {
            Withdraw,
            Deposit
        };

        explicit PageKeypad(PageKeypad::Action action, StateManager *stateManager, QWidget *parent = nullptr);
        ~PageKeypad();

    private slots:
        void onKeypadButtonPress();

    private:
        Ui::PageKeypad *ui;
};

#endif // PAGEKEYPAD_H
