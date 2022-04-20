#ifndef PAGEMAINACCOUNTVIEW_H
#define PAGEMAINACCOUNTVIEW_H

#include "page/abstract/pagewithuserbar.h"

#include <QWidget>

namespace Ui {
    class PageMainAccountView;
}

class PageMainAccountView : public PageWithUserBar {
    Q_OBJECT

    public:
        explicit PageMainAccountView(StateManager *stateManager, QWidget *parent = nullptr);
        ~PageMainAccountView();

    private:
        Ui::PageMainAccountView *ui;
};

#endif // PAGEMAINACCOUNTVIEW_H
