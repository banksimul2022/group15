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

        explicit PageAccountInfo(Action action, StateManager *stateManager, QWidget *parent = nullptr);
        ~PageAccountInfo();

    private:
        Ui::PageAccountInfo *ui;
};

#endif // PAGEACCOUNTINFO_H
