#ifndef PAGERETURN_H
#define PAGERETURN_H

#include <QWidget>

class PageReturn {
    Q_GADGET

    public:
        enum Action {
            LeaveCurrent, AddOnTop
        };

        PageReturn(QWidget *page, Action action);

        QWidget *page();
        Action action();

    private:
        QWidget *pageWidget;
        const Action navigateAction;
};

#endif // PAGERETURN_H
