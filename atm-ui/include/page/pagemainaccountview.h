#ifndef PAGEMAINACCOUNTVIEW_H
#define PAGEMAINACCOUNTVIEW_H

#include "page/abstract/pagewithuserbar.h"

#include <QWidget>
#include <restinfodata.h>

namespace Ui {
    class PageMainAccountView;
}

class PageMainAccountView : public PageWithUserBar {
    Q_OBJECT

    public:
        explicit PageMainAccountView(PageManager *stateManager, QWidget *parent = nullptr);
        ~PageMainAccountView();

        QVariant onNaviagte(const QMetaObject *oldPage, bool closed, QVariant *result) override;
        void onReady() override;

    protected slots:
        PageBase::RestDataAction onRestData(RestReturnData *data) override;

    private slots:
        void on_btnWidthdraw_clicked();
        void on_btnBalance_clicked();
        void on_btnTransactions_clicked();
        void on_btnDeposit_clicked();
        void on_btnTransfer_clicked();
        void on_btnChangePin_clicked();

    private:
            RestInfoData *userInfo;
            Ui::PageMainAccountView *ui;
};

#endif // PAGEMAINACCOUNTVIEW_H
