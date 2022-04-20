#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include <QWidget>
#include <restinterface.h>
#include <rfidinterface.h>

class StateManager {
    public:
        virtual RFIDInterface *getRFIDInterface() = 0;
        virtual RESTInterface *getRESTInterface() = 0;
};

#endif // STATEMANAGER_H
