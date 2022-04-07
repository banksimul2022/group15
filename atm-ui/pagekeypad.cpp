#include "pagekeypad.h"
#include "ui_pagekeypad.h"
#include "userstatusbarwidget.h"

#include <QDebug>

PageKeypad::PageKeypad(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageKeypad)
{
    ui->setupUi(this);
    this->barWidget = new UserStatusBarWidget;

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
