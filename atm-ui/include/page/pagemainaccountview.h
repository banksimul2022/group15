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
        explicit PageMainAccountView(StateManager *stateManager, QWidget *parent = nullptr);
        ~PageMainAccountView();

        bool keepLoadingPageOnNavigate() override;

    protected slots:
        void onRestData(RestReturnData *data) override;

    private:
        Ui::PageMainAccountView *ui;
};

#endif // PAGEMAINACCOUNTVIEW_H
