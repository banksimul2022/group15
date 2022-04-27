#ifndef PAGEACCOUNTINFO_H
#define PAGEACCOUNTINFO_H

#include "page/abstract/pagewithuserbar.h"

#include <QWidget>

namespace Ui {
    class PageAccountInfo;
}

class PageAccountInfo : public PageWithUserBar {
    Q_OBJECT

    public:
        enum Action {
            ViewBalance,
            ViewTransactions
        };

        explicit PageAccountInfo(Action action, RestInfoData *userInfo, StateManager *stateManager, QWidget *parent = nullptr);
        ~PageAccountInfo();

        QVariant onNaviagte(const QMetaObject *oldPage, bool closed, QVariant *result) override;
        void onReady() override;

    protected slots:
        PageBase::RestDataAction onRestData(RestReturnData *data) override;

    private slots:
        void on_btnPrev_clicked();
        void on_btnNext_clicked();

    private:
        void showLoading();
        void showTable();

        Action action;
        Ui::PageAccountInfo *ui;
};

#endif // PAGEACCOUNTINFO_H
