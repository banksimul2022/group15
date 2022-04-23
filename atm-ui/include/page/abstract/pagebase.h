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

        virtual void onNavigate();

        // Return true if this page should be closed
        virtual bool processResult(QWidget *page, QVariant result);

        virtual bool keepLoadingPageOnNavigate();

    protected slots:
        virtual void onRestData(RestReturnData *data);

    protected:
        template <class PageClass, class ...Args> inline void navigate(Args &&... args) {
            this->stateManager->navigateToPage(new PageClass(args..., this->stateManager));
        };

        StateManager *stateManager;
};

#endif // PAGEBASE_H
