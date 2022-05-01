#ifndef PAGERETURN_H
#define PAGERETURN_H

#include "page/abstract/pagebase.h"

#include <QWidget>

class PageReturn {
    Q_GADGET

    public:
        enum Action {
            LeaveCurrent, AddOnTop
        };

        PageReturn(PageBase *page, Action action);

        PageBase *page();
        Action action();

    private:
        PageBase *pageWidget;
        const Action navigateAction;
};

#endif // PAGERETURN_H
