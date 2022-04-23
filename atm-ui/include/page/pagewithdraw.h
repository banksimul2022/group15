#ifndef PAGEWITHDRAW_H
#define PAGEWITHDRAW_H

#include "page/abstract/pagewithuserbar.h"

#include <QWidget>

namespace Ui {
    class PageWithdraw;
}

class PageWithdraw : public PageWithUserBar {
    Q_OBJECT

    public:
        explicit PageWithdraw(RestInfoData *userInfo, StateManager *stateManager, QWidget *parent = nullptr);
        ~PageWithdraw();

    private slots:
        void onAmountButtonPress();

    private:
        Ui::PageWithdraw *ui;
};

#endif // PAGEWITHDRAW_H
