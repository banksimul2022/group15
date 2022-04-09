#include "pagekeypad.h"
#include "ui_pagekeypad.h"
#include "userstatusbarwidget.h"

#include <QDebug>

PageKeypad::PageKeypad(PageKeypad::Action action, StateManager *stateManager, QWidget *parent) :
    PageWithUserBar(stateManager, parent),
    ui(new Ui::PageKeypad)
{
    ui->setupUi(this);

    QMargins margins = this->userStatusBar->contentsMargins();
    margins.setTop(6);
    this->userStatusBar->setContentsMargins(margins);

    this->setupUserBar(this->ui->widgetRootLayout);

    if(action == PageKeypad::Action::Withdraw) {
        this->setWindowTitle(QCoreApplication::translate("PageKeypad", "Nosto - Muu summa", nullptr));
        this->ui->lblAmountText->setText(QCoreApplication::translate("PageKeypad", "Nostettava summa:", nullptr));
        this->ui->btnDot->setVisible(false);
    } else {
        this->setWindowTitle(QCoreApplication::translate("PageKeypad", "Talletus", nullptr));
        this->ui->lblAmountText->setText(QCoreApplication::translate("PageKeypad", "Talletettava summa:", nullptr));
    }

    QList<QPushButton*> gridButtons = this->findChildren<QPushButton*>();

    foreach(QPushButton *btn, gridButtons) {
        // Quick hack to exclude the "Ok" and "Cancel" buttons from having their clicked signals added
        if(btn->text().length() == 1) {
            connect(btn, &QPushButton::clicked, this, &PageKeypad::onKeypadButtonPress);
        }
    }
}

void PageKeypad::onKeypadButtonPress() {
    QPushButton* btn = qobject_cast<QPushButton*>(this->sender());
    qDebug() << btn->text();
    this->userStatusBar->resetTimeout();
}

PageKeypad::~PageKeypad() {
    delete ui;
}
