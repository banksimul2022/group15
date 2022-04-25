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

        bool processResult(QWidget *page, QVariant result) override;

    protected slots:
        void onOk() override;
        void onRestData(RestReturnData *data) override;

    private:
        RestInfoData *userInfo;
        Ui::PageDeposit *ui;
};

#endif // PAGEDEPOSIT_H
