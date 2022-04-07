#include "pagedeposit.h"
#include "ui_pagedeposit.h"
#include "userstatusbarwidget.h"

#include <QDebug>

PageDeposit::PageDeposit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageDeposit)
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
            connect(btn, &QPushButton::clicked, this, &PageDeposit::onKeypadButtonPress);
        }
    }
}

void PageDeposit::onKeypadButtonPress() {
    QPushButton* btn = qobject_cast<QPushButton*>(this->sender());
    qDebug() << btn->text();
    this->barWidget->resetTimeout();
}

PageDeposit::~PageDeposit() {
    delete ui;
}
