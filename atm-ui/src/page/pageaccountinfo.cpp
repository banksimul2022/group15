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

    this->ui->svgLoading->load(QString(":files/icons/spinner.svg"));
    this->ui->svgLoading->renderer()->setAspectRatioMode(Qt::KeepAspectRatio);

    this->ui->tblTransactions->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);

    this->showLoading();

    if(action == Action::ViewBalance) {
        this->setWindowTitle(QCoreApplication::translate("PageAccountInfo", "Saldo", nullptr));
        this->ui->lblTitle->setText(
            QCoreApplication::translate("PageAccountInfo", "%1 %2 tässä saldo tilille %3", nullptr)
                .arg(userInfo->getfName(), userInfo->getlName(), userInfo->getAccountNumber())
        );
        this->ui->btnNext->setVisible(false);
        this->ui->btnPrev->setVisible(false);
    } else {
        this->setWindowTitle(QCoreApplication::translate("PageAccountInfo", "Tapahtumat", nullptr));
        this->ui->lblTitle->setText(
            QCoreApplication::translate("PageAccountInfo", "%1 %2 tässä tapahtumat tilille %3", nullptr)
                .arg(userInfo->getfName(), userInfo->getlName(), userInfo->getAccountNumber())
        );
    }
}

QVariant PageAccountInfo::onNaviagte(const QMetaObject *oldPage, bool closed, QVariant *result) {
    Q_UNUSED(oldPage) Q_UNUSED(result)
    return closed ? QVariant::fromValue(StateManager::Stay) : QVariant::fromValue(StateManager::KeepLoading);
}

void PageAccountInfo::onReady() {
    this->stateManager->getRESTInterface()->getBalance();
}

void PageAccountInfo::onRestData(RestReturnData *data) {
    if(data->type() == RestReturnData::typeBalance) {
        if(data->error() != -1) {
            this->stateManager->leaveCurrentPage(
                QVariant::fromValue(
                    this->stateManager->createPrompt(
                        tr("Verkko virhe!"),
                        tr("Virhe haettaessa tilin tietoja! (%1)").arg(data->error()),
                        PromptEnum::error,
                        0
                    )
                )
            );

            return;
        }

        RestBalanceData *balData = static_cast<RestBalanceData*>(data);
        this->ui->lblAccBalance->setText(QString::number(balData->getBalance(), 'f', 2));
        this->ui->lblAccCredit->setText(QString::number(balData->getCredit(), 'f', 2));

        this->stateManager->leaveLoadingPage();

        RESTInterface *interface = this->stateManager->getRESTInterface(false);

        if(this->action == Action::ViewBalance) {
            interface->latestTransactions(5);
        } else {
            interface->resetTransactionPageIndex();
            interface->nextTransactions(10);
        }
    } else if(data->type() == RestReturnData::typeLatestTransaction || data->type() == RestReturnData::typeTransaction) {
        if(data->error() != -1) {
            this->stateManager->leaveCurrentPage(
                QVariant::fromValue(
                    this->stateManager->createPrompt(
                        tr("Verkko virhe!"),
                        tr("Virhe haettaessa tilin tapahtumia! (%1)").arg(data->error()),
                        PromptEnum::error,
                        0
                    )
                )
            );
            return;
        }

        RestTransactionData *transactionData = static_cast<RestTransactionData*>(data);
        QAbstractItemModel *oldModel = this->ui->tblTransactions->model();
        this->ui->tblTransactions->setModel(new TransactionTableModel(transactionData, this->ui->tblTransactions));
        delete oldModel;

        if(data->type() == RestReturnData::typeTransaction) {
            this->ui->btnPrev->setEnabled(transactionData->getHasPrev());
            this->ui->btnNext->setEnabled(transactionData->getHasNext());
            this->userStatusBar->startLeaveTimeout();
        }

        this->showTable();
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
