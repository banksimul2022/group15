#ifndef PAGEKEYPAD_H
#define PAGEKEYPAD_H

#include "page/abstract/pagewithuserbar.h"
#include "userstatusbarwidget.h"

#include <QWidget>

namespace Ui {
    class PageKeypad;
}

class PageKeypad : public PageWithUserBar {
    Q_OBJECT

    public:
        explicit PageKeypad(bool showDot, QString title, QString message, RestInfoData *userInfo, PageManager *pageManager, QWidget *parent = nullptr);
        ~PageKeypad();

    protected slots:
        void onLeave() override;
        void onOk() override;

    private slots:
        void onKeypadButtonPress();
        void onFlashTick();

    private:
        void flash(ushort count);

        Ui::PageKeypad *ui;
        QTimer *flashTimer;
        ushort flashCount;
        bool flashState;
};

#endif // PAGEKEYPAD_H
