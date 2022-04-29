#ifndef PAGEMANAGER_H
#define PAGEMANAGER_H

#include <QWidget>
#include <restinterface.h>
#include <asyncserialinterface.h>

#include "page/pageprompt_enum.h"

class PageManager {
    Q_GADGET

    public:
        enum PageReturnAction {
            Stay, Leave, KeepLoading
        };
        Q_ENUM(PageReturnAction);

        virtual AsyncSerialInterface *getRFIDInterface() = 0;

        virtual RESTInterface *getRESTInterface(bool displayLoadingPage = true) = 0;

        // Convenience method for displayting messages
        virtual QWidget *createPrompt(QString title, QString message, PromptEnum::Icon icon, int btnCount, ...) = 0;
        virtual QWidget *createPrompt(QString title, QString message, PromptEnum::Icon icon, int btnCount, va_list args) = 0;
        virtual void displayPrompt(QString title, QString message, PromptEnum::Icon icon, int btnCount, ...) = 0;

        virtual void leaveLoadingPage() = 0;

        virtual QVariant navigateToPage(QWidget *page) = 0;
        virtual bool leaveCurrentPage(QVariant result) = 0;
        virtual void leaveAllPages(QVariant result) = 0;
};

#endif // PAGEMANAGER_H
