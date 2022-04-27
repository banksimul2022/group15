#include "page/pagedeposit.h"
#include "ui_pagedeposit.h"
#include "page/pagekeypad.h"
#include "utility.h"

PageDeposit::PageDeposit(RestInfoData *userInfo, PageManager *stateManager, QWidget *parent) :
    PageWithUserBar(UserStatusBarWidget::leaveAndOk, stateManager, userInfo, parent),
    amountDeposited(0),
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
            return QVariant::fromValue(PageManager::Leave);
        }

        this->amountDeposited = result->toDouble();
        this->pageManager->getRESTInterface(false)->deposit(this->amountDeposited);
        return QVariant::fromValue(PageManager::KeepLoading);
    }

    return QVariant::fromValue(PageManager::Stay);
}

PageBase::RestDataAction PageDeposit::onRestData(RestReturnData *data) {
    if(data->type() != RestReturnData::typeDeposit) {
        return RestDataAction::Skip;
    }

    if(this->handleRestError(data, tr("talletettaessa"))) {
        return RestDataAction::Delete;
    }

    this->pageManager->leaveCurrentPage(
        QVariant::fromValue(
            this->pageManager->createPrompt(
                tr("Talletus onnistui"),
                tr("Talletit %1€").arg(this->amountDeposited),
                PromptEnum::info,
                0
            )
        )
    );

    return RestDataAction::Delete;
}

PageDeposit::~PageDeposit() {
    delete ui;
}
