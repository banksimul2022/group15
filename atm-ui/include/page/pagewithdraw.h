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

        QVariant onNaviagte(const QMetaObject *oldPage, bool closed, QVariant *result) override;
        void onReady() override;

    private slots:
        PageBase::RestDataAction onRestData(RestReturnData *data) override;
        void onAmountButtonPress();

    private:
        bool useCredit;
        double amountWithdrawn;
        RestInfoData *userInfo;
        Ui::PageWithdraw *ui;
};

#endif // PAGEWITHDRAW_H
