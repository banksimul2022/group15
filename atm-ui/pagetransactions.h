#ifndef PAGETRANSACTIONS_H
#define PAGETRANSACTIONS_H

#include <QWidget>

namespace Ui {
    class PageTransactions;
}

class PageTransactions : public QWidget {
    Q_OBJECT

    public:
        explicit PageTransactions(QWidget *parent = nullptr);
        ~PageTransactions();

    private:
        Ui::PageTransactions *ui;
};

#endif // PAGETRANSACTIONS_H
