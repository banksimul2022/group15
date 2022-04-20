#ifndef PAGEBASE_H
#define PAGEBASE_H

#include "statemanager.h"

#include <QWidget>

class PageBase : public QWidget {
    Q_OBJECT

    public:
        explicit PageBase(StateManager *stateManager, QWidget *parent = nullptr);
        virtual ~PageBase() = 0;

    protected:
        StateManager *stateManager;
};

#endif // PAGEBASE_H
