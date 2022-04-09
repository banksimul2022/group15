#ifndef PAGEINSERTCARD_H
#define PAGEINSERTCARD_H

#include "pagebase.h"

#include <QWidget>

namespace Ui {
    class PageInsertCard;
}

class PageInsertCard : public PageBase {
    Q_OBJECT

    public:
        explicit PageInsertCard(StateManager *stateManager, QWidget *parent = nullptr);
        ~PageInsertCard();

    private:
        Ui::PageInsertCard *ui;
};

#endif // PAGEINSERTCARD_H
