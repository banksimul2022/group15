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

        void onNavigate() override;
        bool processResult(QWidget *page, QVariant result) override;

    private slots:
        void onRestData(RestReturnData *data) override;
        void onAmountButtonPress();

    private:
        bool useCredit;
        RestInfoData *userInfo;
        Ui::PageWithdraw *ui;
};

#endif // PAGEWITHDRAW_H
