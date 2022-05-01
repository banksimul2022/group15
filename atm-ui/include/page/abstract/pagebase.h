#ifndef PAGEBASE_H
#define PAGEBASE_H

#include "pagemanager.h"

#include <QVariant>
#include <QWidget>

class PageBase : public QWidget {
    Q_OBJECT

    public:
        enum RestDataAction {
            Delete, SetNull, Skip
        };

        explicit PageBase(PageManager *pageManager, QWidget *parent = nullptr);
        virtual ~PageBase() = 0;

        virtual QVariant onNaviagte(const QMetaObject *oldPage, bool closed, QVariant *result);
        virtual void onShown(); // Called everytime this page is show

        virtual RestDataAction onRestData(RestReturnData *data);

    protected:
        bool handleRestError(RestReturnData *data, QString action, bool leave = true);

        virtual void onReady(); // Called when page is shown for the first time

        template <class PageClass, class ...Args> inline void navigate(Args &&... args) {
            this->pageManager->navigateToPage(new PageClass(args..., this->pageManager));
        };

        PageManager *pageManager;

    private:
        bool hasBeenShown;
};

#endif // PAGEBASE_H
