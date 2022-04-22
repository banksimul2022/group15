#ifndef PAGEWITHUSERBAR_H
#define PAGEWITHUSERBAR_H

#include "pagebase.h"
#include "userstatusbarwidget.h"

#include <QHBoxLayout>

class PageWithUserBar : public PageBase {
    Q_OBJECT

    public:
        explicit PageWithUserBar(UserStatusBarWidget::Mode mode, StateManager *stateManager, QWidget *parent = nullptr);
        virtual ~PageWithUserBar() = 0;

    protected:
        void setupUserBar(QLayout *layout);
        UserStatusBarWidget * userStatusBar;

    protected slots:
        void onRestData(RestReturnData *data) override;

        virtual void onExtraButton(int id);
        virtual void onLeave();
        virtual void onOk();
};

#endif // PAGEWITHUSERBAR_H
