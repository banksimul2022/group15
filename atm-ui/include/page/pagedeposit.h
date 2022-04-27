#ifndef PAGEDEPOSIT_H
#define PAGEDEPOSIT_H

#include "page/abstract/pagewithuserbar.h"

namespace Ui {
    class PageDeposit;
}

class PageDeposit : public PageWithUserBar {
    Q_OBJECT

    public:
        explicit PageDeposit(RestInfoData *userInfo, StateManager *stateManager, QWidget *parent = nullptr);
        ~PageDeposit();

        QVariant onNaviagte(const QMetaObject *oldPage, bool closed, QVariant *result) override;

    protected slots:
        void onOk() override;
        PageBase::RestDataAction onRestData(RestReturnData *data) override;

    private:
        double amountDeposited;
        RestInfoData *userInfo;
        Ui::PageDeposit *ui;
};

#endif // PAGEDEPOSIT_H
