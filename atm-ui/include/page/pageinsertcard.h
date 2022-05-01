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
        explicit PageInsertCard(PageManager *pageManager, QWidget *parent = nullptr);
        ~PageInsertCard();

        QVariant onNaviagte(const QMetaObject *oldPage, bool closed, QVariant *result) override;

    private slots:
        void onCardRead(QString number);

        void onPinRead(QString pin);
        void onPinCancel();

        PageBase::RestDataAction onRestData(RestReturnData *data) override;

    private:
        QString number;
        QString pin;
        bool processReads;

        Ui::PageInsertCard *ui;
};

#endif // PAGEINSERTCARD_H
