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

        bool keepLoadingPageOnNavigate() override;

    protected slots:
        void onRestData(RestReturnData *data) override;

    private:
        Ui::PageAccountInfo *ui;
};

#endif // PAGEACCOUNTINFO_H
