#include "page/pagekeypad.h"
#include "ui_pagekeypad.h"
#include "userstatusbarwidget.h"
#include "utility.h"

#include <QDebug>
#include <QTimer>
#include <QStyle>

PageKeypad::PageKeypad(PageKeypad::Action action, StateManager *stateManager, QWidget *parent) :
    PageWithUserBar(UserStatusBarWidget::Mode::leaveAndOk, stateManager, nullptr, parent),
    ui(new Ui::PageKeypad),
    flashTimer(new QTimer(this)),
    flashCount(0)
{
    ui->setupUi(this);

    QMargins margins = this->userStatusBar->contentsMargins();
    margins.setTop(6);
    this->userStatusBar->setContentsMargins(margins);

    this->setupUserBar(this->ui->widgetRootLayout);

    this->setStyleSheet("QLabel[flash=\"true\"] { color: red; }");

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
        if(btn->parent() == this) {
            connect(btn, &QPushButton::clicked, this, &PageKeypad::onKeypadButtonPress);
        }
    }

    this->flashTimer->setInterval(80);
    this->connect(this->flashTimer, &QTimer::timeout, this, &PageKeypad::onFlashTick);
}

void PageKeypad::onLeave() {
    this->stateManager->leaveCurrentPage(QVariant(false));
}

void PageKeypad::onOk() {
    QString sumStr = this->ui->lblAmount->text();

    if(sumStr.length() < 1) {
        this->flash(5);
        return;
    }

    bool ok = false;
    double sum = sumStr.toDouble(&ok);
    Q_ASSERT(ok); // TODO: Temporary until i get the error page setup
    this->stateManager->leaveCurrentPage(QVariant(sum));
}

void PageKeypad::onKeypadButtonPress() {
    this->userStatusBar->resetLeaveTimeout();

    QString name = this->sender()->objectName();
    QString currentSum = this->ui->lblAmount->text();

    if(name.startsWith("btnDigit")) {
        QString valStr = name.remove(0, 8);
        int dotIndex = currentSum.indexOf('.');

        if((valStr == "0" && currentSum.length() < 1) || (dotIndex != -1 && currentSum.length() - dotIndex > 2)) {
            this->flash(5);
            return;
        }

        this->ui->lblAmount->setText(currentSum + valStr);
    } else if(name == "btnDot") {
        if(currentSum.length() < 1 || currentSum.contains('.')) {
            this->flash(5);
            return;
        }

        this->ui->lblAmount->setText(currentSum + '.');
    } else if(name == "btnClear") {
        this->ui->lblAmount->setText("");
    } else {
        // Code execution SHOULD not get here
        Q_ASSERT(false);
    }
}

void PageKeypad::onFlashTick() {
    QVariant f(false);

    if(this->flashCount-- < 1) {
        this->flashTimer->stop();
    } else {
        f.setValue<bool>(this->ui->lblAmount->property("flash").toBool() ^ 1); // Flip the value (0 -> 1 and 1 -> 0)
    }

    Utility::updateVisualProperty("flash", f, this->ui->lblAmount);
    Utility::updateVisualProperty("flash", f, this->ui->lblAmountText);
}

void PageKeypad::flash(ushort count) {
    if(count == 0) return;
    this->flashCount = count;
    this->flashTimer->start();
}

PageKeypad::~PageKeypad() {
    delete ui;
}
