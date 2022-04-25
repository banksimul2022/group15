#ifndef PAGEINSERTCARD_H
#define PAGEINSERTCARD_H

#include "page/abstract/pagebase.h"

#include <QWidget>

namespace Ui {
    class PageInsertCard;
}

class PageInsertCard : public PageBase {
    Q_OBJECT

    public:
        explicit PageInsertCard(StateManager *stateManager, QWidget *parent = nullptr);
        ~PageInsertCard();

        bool processResult(QWidget *page, QVariant result) override;

    private slots:
        void onCardRead(QString number);
        void onRestData(RestReturnData *data) override;

    private:
        bool processReads;

        Ui::PageInsertCard *ui;
};

#endif // PAGEINSERTCARD_H
