#include "page/pageaccountinfo.h"
#include "ui_pageaccountinfo.h"
#include "userstatusbarwidget.h"

#include <restbalancedata.h>
#include <resttransactiondata.h>
#include <transactiontablemodel.h>
#include <QSvgRenderer>

PageAccountInfo::PageAccountInfo(Action action, RestInfoData *userInfo, StateManager *stateManager, QWidget *parent) :
    PageWithUserBar(UserStatusBarWidget::Mode::leaveOnly, stateManager, nullptr, parent),
    action(action),
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
        this->showTable();

        this->stateManager->getRESTInterface()->getBalance();
    } else {
        this->setWindowTitle(QCoreApplication::translate("PageAccountInfo", "Tapahtumat", nullptr));
        this->ui->lblTitle->setText(QCoreApplication::translate("PageAccountInfo", "Tapahtumat tilille %1", nullptr).arg(userInfo->getAccountNumber()));

        this->ui->svgLoading->load(QString(":files/icons/spinner.svg"));
        this->ui->svgLoading->renderer()->setAspectRatioMode(Qt::KeepAspectRatio);
        this->showLoading();

        QList<QLabel*> labels = this->findChildren<QLabel*>();

        foreach(QLabel *label, labels) {
            if(!label->objectName().startsWith("lblAcc")) continue;
            label->setVisible(false);
        }

        RESTInterface *interface = this->stateManager->getRESTInterface(false);
        interface->resetTransactionPageIndex();
        interface->nextTransactions(10);
    }
}

bool PageAccountInfo::keepLoadingPageOnNavigate() { return this->action == Action::ViewBalance; }

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
    } else if(data->type() == RestReturnData::typeLatestTransaction || data->type() == RestReturnData::typeTransaction) {
        if(data->error() != -1) {
            // TODO: Add error prompt
            qDebug() << "ERROR getting transactions" << data->error();
            this->stateManager->leaveCurrentPage(QVariant());
            return;
        }

        RestTransactionData *transactionData = static_cast<RestTransactionData*>(data);
        QAbstractItemModel *oldModel = this->ui->tblTransactions->model();
        this->ui->tblTransactions->setModel(new TransactionTableModel(transactionData, this->ui->tblTransactions));
        delete oldModel;

        if(data->type() == RestReturnData::typeLatestTransaction) {
            this->stateManager->leaveLoadingPage();
        } else {
            this->ui->btnPrev->setEnabled(transactionData->getHasPrev());
            this->ui->btnNext->setEnabled(transactionData->getHasNext());
            this->showTable();
            this->userStatusBar->startLeaveTimeout();
        }
    }
}

void PageAccountInfo::showLoading() {
    this->ui->svgLoading->setVisible(true);
    this->ui->tblTransactions->setVisible(false);
    this->ui->btnPrev->setEnabled(false);
    this->ui->btnNext->setEnabled(false);
}

void PageAccountInfo::showTable() {
    this->ui->svgLoading->setVisible(false);
    this->ui->tblTransactions->setVisible(true);
}

PageAccountInfo::~PageAccountInfo() {
    delete ui;
}

void PageAccountInfo::on_btnPrev_clicked() {
    this->showLoading();
    this->userStatusBar->stopLeaveTimeout();
    this->userStatusBar->resetLeaveTimeout(false);
    this->stateManager->getRESTInterface(false)->prevTransactions(10);
}

void PageAccountInfo::on_btnNext_clicked() {
    this->showLoading();
    this->userStatusBar->stopLeaveTimeout();
    this->userStatusBar->resetLeaveTimeout(false);
    this->stateManager->getRESTInterface(false)->nextTransactions(10);
}
