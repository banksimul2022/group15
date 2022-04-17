#ifndef PAGEACCOUNTINFO_H
#define PAGEACCOUNTINFO_H

#include "pagewithuserbar.h"

#include <QWidget>

namespace Ui {
    class PageAccountInfo;
}

class PageAccountInfo : public PageWithUserBar {
    Q_OBJECT

    public:
        explicit PageAccountInfo(StateManager *stateManager, QWidget *parent = nullptr);
        ~PageAccountInfo();

    private:
        Ui::PageAccountInfo *ui;
};

#endif // PAGEACCOUNTINFO_H
