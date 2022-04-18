#ifndef PAGELOADING_H
#define PAGELOADING_H

#include "pagebase.h"

#include <QWidget>

namespace Ui {
    class PageLoading;
}

class PageLoading : public PageBase {
    Q_OBJECT

    public:
        explicit PageLoading(StateManager *stateManager, QWidget *parent = nullptr);
        ~PageLoading();

    private:
        Ui::PageLoading *ui;
};

#endif // PAGELOADING_H
