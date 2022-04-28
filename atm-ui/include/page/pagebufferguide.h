#ifndef PAGEBUFFERGUIDE_H
#define PAGEBUFFERGUIDE_H

#include "page/abstract/pagewithuserbar.h"

namespace Ui {
    class PageBufferGuide;
}

class PageBufferGuide : public PageWithUserBar {
    Q_OBJECT

    public:
        enum Action {
            Deposit, Transfer
        };

        explicit PageBufferGuide(Action action, RestInfoData *userInfo, PageManager *pageManager, QWidget *parent = nullptr);
        ~PageBufferGuide();

        QVariant onNaviagte(const QMetaObject *oldPage, bool closed, QVariant *result) override;

    protected slots:
        void onOk() override;
        PageBase::RestDataAction onRestData(RestReturnData *data) override;

    private:
        Action action;
        QString targetAccount;
        double amount;
        RestInfoData *userInfo;
        Ui::PageBufferGuide *ui;
};

#endif // PAGEBUFFERGUIDE_H
