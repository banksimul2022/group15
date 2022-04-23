#include "page/pagewithdraw.h"
#include "ui_pagewithdraw.h"
#include "userstatusbarwidget.h"

#include <QDebug>

PageWithdraw::PageWithdraw(RestInfoData *userInfo, StateManager *stateManager, QWidget *parent) :
    PageWithUserBar(UserStatusBarWidget::leaveOnly, stateManager, userInfo, parent),
    ui(new Ui::PageWithdraw)
{
    ui->setupUi(this);
    this->setupUserBar(this->ui->widgetRootLayout);

    QList<QPushButton*> withdrawButtons = this->findChildren<QPushButton*>();

    foreach(QPushButton *btn, withdrawButtons) {
        if(btn->parent() == this) {
            connect(btn, &QPushButton::clicked, this, &PageWithdraw::onAmountButtonPress);
        }
    }
}

void PageWithdraw::onAmountButtonPress() {
    QString name = this->sender()->objectName();

    if(name.startsWith("btnAmount")) {
        bool ok;
        uint val = name.remove(0, 9).toUInt(&ok);
        Q_ASSERT(ok);
        qDebug() << val;
    } else if(name == "btnOther") {
        qDebug() << "Other";
    } else {
        // Code execution SHOULD not get here
        Q_ASSERT(false);
    }
}

PageWithdraw::~PageWithdraw() {
    delete ui;
}
