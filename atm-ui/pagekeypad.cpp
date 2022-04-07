#include "pagekeypad.h"
#include "ui_pagekeypad.h"
#include "userstatusbarwidget.h"

#include <QDebug>

PageKeypad::PageKeypad(PageKeypad::Action action, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageKeypad)
{
    ui->setupUi(this);
    this->barWidget = new UserStatusBarWidget;

    if(action == PageKeypad::Action::Withdraw) {
        this->setWindowTitle(QCoreApplication::translate("PageKeypad", "Nosto - Muu summa", nullptr));
        this->ui->lblAmountText->setText(QCoreApplication::translate("PageKeypad", "Nostettava summa:", nullptr));
        this->ui->btnDot->setVisible(false);
    } else {
        this->setWindowTitle(QCoreApplication::translate("PageKeypad", "Talletus", nullptr));
        this->ui->lblAmountText->setText(QCoreApplication::translate("PageKeypad", "Talletettava summa:", nullptr));
    }

    QMargins margins = this->barWidget->contentsMargins();
    margins.setTop(6);
    this->barWidget->setContentsMargins(margins);

    this->ui->widgetRootLayout->addWidget(this->barWidget);

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
    this->barWidget->resetTimeout();
}

PageKeypad::~PageKeypad() {
    delete ui;
}
