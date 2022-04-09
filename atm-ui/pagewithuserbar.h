#ifndef PAGEWITHUSERBAR_H
#define PAGEWITHUSERBAR_H

#include "pagebase.h"
#include "userstatusbarwidget.h"

#include <QHBoxLayout>

class PageWithUserBar : public PageBase {
    Q_OBJECT

    public:
        explicit PageWithUserBar(StateManager *stateManager, QWidget *parent = nullptr);
        virtual ~PageWithUserBar() = 0;

    protected:
        void setupUserBar(QLayout *layout);
        UserStatusBarWidget * userStatusBar;

    private slots:
        void logout();
};

#endif // PAGEWITHUSERBAR_H
