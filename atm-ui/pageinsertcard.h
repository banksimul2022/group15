#ifndef PAGEINSERTCARD_H
#define PAGEINSERTCARD_H

#include <QWidget>

namespace Ui {
    class PageInsertCard;
}

class PageInsertCard : public QWidget {
    Q_OBJECT

    public:
        explicit PageInsertCard(QWidget *parent = nullptr);
        ~PageInsertCard();

    private:
        Ui::PageInsertCard *ui;
};

#endif // PAGEINSERTCARD_H
