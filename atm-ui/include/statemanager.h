#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include <QWidget>

class StateManager {
    public:
        virtual void logout() = 0;
        virtual void setPage(QWidget *page) = 0;
};

#endif // STATEMANAGER_H
