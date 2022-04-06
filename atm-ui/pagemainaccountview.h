#ifndef PAGEMAINACCOUNTVIEW_H
#define PAGEMAINACCOUNTVIEW_H

#include <QWidget>

namespace Ui {
    class PageMainAccountView;
}

class PageMainAccountView : public QWidget {
    Q_OBJECT

    public:
        explicit PageMainAccountView(QWidget *parent = nullptr);
        ~PageMainAccountView();

    private:
        Ui::PageMainAccountView *ui;
};

#endif // PAGEMAINACCOUNTVIEW_H
