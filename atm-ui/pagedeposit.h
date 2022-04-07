#ifndef PAGEDEPOSIT_H
#define PAGEDEPOSIT_H

#include "userstatusbarwidget.h"

#include <QWidget>

namespace Ui {
    class PageDeposit;
}

class PageDeposit : public QWidget {
    Q_OBJECT

    public:
        explicit PageDeposit(QWidget *parent = nullptr);
        ~PageDeposit();

    private slots:
        void onKeypadButtonPress();

    private:
        Ui::PageDeposit *ui;
        UserStatusBarWidget *barWidget;
};

#endif // PAGEDEPOSIT_H
