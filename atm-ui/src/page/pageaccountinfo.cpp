#include "page/pageaccountinfo.h"
#include "ui_pageaccountinfo.h"
#include "userstatusbarwidget.h"

#include <restbalancedata.h>
#include <resttransactiondata.h>
#include <transactiontablemodel.h>

PageAccountInfo::PageAccountInfo(Action action, RestInfoData *userInfo, StateManager *stateManager, QWidget *parent) :
    PageWithUserBar(UserStatusBarWidget::Mode::leaveOnly, stateManager, nullptr, parent),
    ui(new Ui::PageAccountInfo)
{
    ui->setupUi(this);
    this->setupUserBar(this->ui->widgetRootLayout);

    this->ui->tblTransactions->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);

    if(action == Action::ViewBalance) {
        this->setWindowTitle(QCoreApplication::translate("PageAccountInfo", "Saldo", nullptr));
        this->ui->lblTitle->setText(QCoreApplication::translate("PageAccountInfo", "Saldo tilille %1", nullptr).arg(userInfo->getAccountNumber()));
        this->ui->btnNext->setVisible(false);
        this->ui->btnPrev->setVisible(false);
    } else {
        this->setWindowTitle(QCoreApplication::translate("PageAccountInfo", "Tapahtumat", nullptr));
        this->ui->lblTitle->setText(QCoreApplication::translate("PageAccountInfo", "Tapahtumat tilille %1", nullptr).arg(userInfo->getAccountNumber()));

        QList<QLabel*> labels = this->findChildren<QLabel*>();

        foreach(QLabel *label, labels) {
            if(!label->objectName().startsWith("lblAcc")) continue;
            label->setVisible(false);
        }
    }

    this->stateManager->getRESTInterface()->getBalance();
}

bool PageAccountInfo::keepLoadingPageOnNavigate() { return true; }

void PageAccountInfo::onRestData(RestReturnData *data) {
    if(data->type() == RestReturnData::typeBalance) {
        if(data->error() != -1) {
            // TODO: Add error prompt
            qDebug() << "ERROR getting balance" << data->error();
            this->stateManager->leaveCurrentPage(QVariant());
            return;
        }

        RestBalanceData *balData = static_cast<RestBalanceData*>(data);
        this->ui->lblAccBalance->setText(QString::number(balData->getBalance(), 'f', 2));
        this->ui->lblAccCredit->setText(QString::number(balData->getCredit(), 'f', 2));
        this->stateManager->getRESTInterface()->latestTransactions(5);
    } else if(data->type() == RestReturnData::typeLatestTransaction) {
        if(data->error() != -1) {
            // TODO: Add error prompt
            qDebug() << "ERROR getting transactions" << data->error();
            this->stateManager->leaveCurrentPage(QVariant());
            return;
        }

        this->ui->tblTransactions->setModel(new TransactionTableModel(static_cast<RestTransactionData*>(data), this->ui->tblTransactions));
        this->stateManager->leaveLoadingPage();
    }
}

PageAccountInfo::~PageAccountInfo() {
    delete ui;
}
