#include "pagewithdraw.h"
#include "ui_pagewithdraw.h"
#include "userstatusbarwidget.h"

#include <QDebug>

PageWithdraw::PageWithdraw(StateManager *stateManager, QWidget *parent) :
    PageWithUserBar(stateManager, parent),
    ui(new Ui::PageWithdraw)
{
    ui->setupUi(this);
    this->setupUserBar(this->ui->widgetRootLayout);

    QList<QPushButton*> withdrawButtons = this->findChildren<QPushButton*>();

    foreach(QPushButton *btn, withdrawButtons) {
        if(btn->objectName() != "btnLogout") {
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
    } else if(name == "btnCancel") {
        qDebug() << "Cancel";
    } else {
        // Code execution SHOULD not get here
        Q_ASSERT(false);
    }
}

PageWithdraw::~PageWithdraw() {
    delete ui;
}
