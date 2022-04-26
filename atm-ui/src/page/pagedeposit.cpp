#include "page/pagedeposit.h"
#include "ui_pagedeposit.h"
#include "page/pagekeypad.h"
#include "utility.h"

PageDeposit::PageDeposit(RestInfoData *userInfo, StateManager *stateManager, QWidget *parent) :
    PageWithUserBar(UserStatusBarWidget::leaveAndOk, stateManager, userInfo, parent),
    userInfo(userInfo),
    ui(new Ui::PageDeposit)
{
    ui->setupUi(this);
    this->setupUserBar(this->ui->widgetRootLayout);
}

void PageDeposit::onOk() {
    this->navigate<PageKeypad>(PageKeypad::Deposit, this->userInfo);
}

QVariant PageDeposit::onNaviagte(const QMetaObject *oldPage, bool closed, QVariant *result) {
    if(closed) {
        // If result is bool it will always be false (User canceled action)
        if(
               !oldPage->inherits(&PageKeypad::staticMetaObject) ||
               result->type() == QVariant::Bool ||
               result->type() != QVariant::Double
          ) {
            return QVariant::fromValue(StateManager::Leave);
        }

        this->stateManager->getRESTInterface(false)->deposit(result->toDouble());
        return QVariant::fromValue(StateManager::KeepLoading);
    }

    return QVariant::fromValue(StateManager::Stay);
}

void PageDeposit::onRestData(RestReturnData *data) {
    if(data->type() != RestReturnData::typeDeposit) {
        return;
    }

    if(data->error() != -1) {
        // TODO: Replace with prompt
        qDebug() << "ERROR depositing sum" << data->error();
    }

    delete data;
    this->stateManager->leaveCurrentPage(QVariant::fromValue(StateManager::Leave));
}

PageDeposit::~PageDeposit() {
    delete ui;
}
