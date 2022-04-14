#include "pagekeypad.h"
#include "ui_pagekeypad.h"
#include "userstatusbarwidget.h"

#include <QDebug>

PageKeypad::PageKeypad(PageKeypad::Action action, StateManager *stateManager, QWidget *parent) :
    PageWithUserBar(UserStatusBarWidget::Mode::leaveAndOk, stateManager, parent),
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
    this->userStatusBar->resetLeaveTimeout();

    QString name = this->sender()->objectName();

    if(name.startsWith("btnDigit")) {
        bool ok;
        uint val = name.remove(0, 8).toUInt(&ok);
        Q_ASSERT(ok);
        qDebug() << val;
    } else {
        // Code execution SHOULD not get here
        Q_ASSERT(false);
    }
}

PageKeypad::~PageKeypad() {
    delete ui;
}
