#ifndef PAGEWITHDRAW_H
#define PAGEWITHDRAW_H

#include <QWidget>

namespace Ui {
    class PageWithdraw;
}

class PageWithdraw : public QWidget {
    Q_OBJECT

    public:
        explicit PageWithdraw(QWidget *parent = nullptr);
        ~PageWithdraw();

    private slots:
        void onAmountButtonPress();

    private:
        Ui::PageWithdraw *ui;
};

#endif // PAGEWITHDRAW_H
