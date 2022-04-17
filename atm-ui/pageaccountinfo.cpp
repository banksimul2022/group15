#include "pageaccountinfo.h"
#include "ui_pageaccountinfo.h"
#include "userstatusbarwidget.h"

PageAccountInfo::PageAccountInfo(Action action, StateManager *stateManager, QWidget *parent) :
    PageWithUserBar(UserStatusBarWidget::Mode::leaveOnly, stateManager, parent),
    ui(new Ui::PageAccountInfo)
{
    ui->setupUi(this);
    this->setupUserBar(this->ui->widgetRootLayout);

    if(action == Action::ViewBalance) {
        this->setWindowTitle(QCoreApplication::translate("PageAccountInfo", "Saldo", nullptr));
        this->ui->lblTitle->setText(QCoreApplication::translate("PageAccountInfo", "Saldo tilille %1", nullptr));
        this->ui->btnNext->setVisible(false);
        this->ui->btnPrev->setVisible(false);
    } else {
        this->setWindowTitle(QCoreApplication::translate("PageAccountInfo", "Tapahtumat", nullptr));
        this->ui->lblTitle->setText(QCoreApplication::translate("PageAccountInfo", "Tapahtumat tilille %1", nullptr));

        QList<QLabel*> labels = this->findChildren<QLabel*>();

        foreach(QLabel *label, labels) {
            if(!label->objectName().startsWith("lblAcc")) continue;
            label->setVisible(false);
        }
    }
}

PageAccountInfo::~PageAccountInfo() {
    delete ui;
}
