#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include <QWidget>
#include <restinterface.h>
#include <rfidinterface.h>

class StateManager {
    public:
        virtual RFIDInterface *getRFIDInterface() = 0;
        virtual RESTInterface *getRESTInterface(bool displayLoadingPage = true) = 0;

        virtual void navigateToPage(QWidget *page) = 0;
        virtual bool leaveCurrentPage(QVariant result) = 0;
};

#endif // STATEMANAGER_H
