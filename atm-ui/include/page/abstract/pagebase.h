#ifndef PAGEBASE_H
#define PAGEBASE_H

#include "statemanager.h"

#include <QVariant>
#include <QWidget>

class PageBase : public QWidget {
    Q_OBJECT

    public:
        explicit PageBase(StateManager *stateManager, QWidget *parent = nullptr);
        virtual ~PageBase() = 0;

        virtual QVariant onNaviagte(const QMetaObject *oldPage, bool closed, QVariant *result);
        virtual void onShown(); // Called everytime this page is show

    protected:
        enum RestDataAction {
            Delete, SetNull, Skip
        };

        virtual RestDataAction onRestData(RestReturnData *data);
        bool handleRestError(RestReturnData *data, QString action, bool leave = true);

        virtual void onReady(); // Called when page is shown for the first time

        template <class PageClass, class ...Args> inline void navigate(Args &&... args) {
            this->stateManager->navigateToPage(new PageClass(args..., this->stateManager));
        };

        StateManager *stateManager;

    private slots:
        virtual void onRestDataFromManager(RestReturnData **data);

    private:
        bool hasBeenShown;
};

#endif // PAGEBASE_H
