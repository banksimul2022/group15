#ifndef PAGETRANSACTIONS_H
#define PAGETRANSACTIONS_H

#include "pagewithuserbar.h"

#include <QWidget>

namespace Ui {
    class PageTransactions;
}

class PageTransactions : public PageWithUserBar {
    Q_OBJECT

    public:
        explicit PageTransactions(StateManager *stateManager, QWidget *parent = nullptr);
        ~PageTransactions();

    private:
        Ui::PageTransactions *ui;
};

#endif // PAGETRANSACTIONS_H
