#include "page/pagedeposit.h"
#include "ui_pagedeposit.h"
#include "page/pagekeypad.h"
#include "utility.h"

PageDeposit::PageDeposit(RestInfoData *userInfo, StateManager *stateManager, QWidget *parent) :
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
            return QVariant::fromValue(StateManager::Leave);
        }

        this->amountDeposited = result->toDouble();
        this->stateManager->getRESTInterface(false)->deposit(this->amountDeposited);
        return QVariant::fromValue(StateManager::KeepLoading);
    }

    return QVariant::fromValue(StateManager::Stay);
}

void PageDeposit::onRestData(RestReturnData *data) {
    if(data->type() != RestReturnData::typeDeposit) {
        return;
    }

    if(data->error() != -1) {
        this->stateManager->leaveCurrentPage(
            QVariant::fromValue(
                this->stateManager->createPrompt(
                    tr("Sisäinen virhe!"),
                    tr("Virhe talletettessa! (%1)").arg(data->error()),
                    PromptEnum::error,
                    0
                )
            )
        );

        delete data;
        return;
    }

    delete data;
    this->stateManager->leaveCurrentPage(
        QVariant::fromValue(
            this->stateManager->createPrompt(
                tr("Talletus Onnistui"),
                tr("Talletit %1€").arg(this->amountDeposited),
                PromptEnum::info,
                0
            )
        )
    );
}

PageDeposit::~PageDeposit() {
    delete ui;
}
