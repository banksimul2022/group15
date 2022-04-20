#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include <QWidget>
#include <restinterface.h>
#include <rfidinterface.h>

#include "page/pageprompt_enum.h"

class StateManager {
    public:
        virtual RFIDInterface *getRFIDInterface() = 0;
        virtual RESTInterface *getRESTInterface(bool displayLoadingPage = true) = 0;

        // Convenience method for displayting messages
        virtual void displayPrompt(QObject *ctx, const char *title, const char *message, PromptEnum::Icon icon, int btnCount, ...) = 0;

        virtual void navigateToPage(QWidget *page) = 0;
        virtual bool leaveCurrentPage(QVariant result) = 0;
};

#endif // STATEMANAGER_H
