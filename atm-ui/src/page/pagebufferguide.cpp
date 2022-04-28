#include "page/pagebufferguide.h"
#include "ui_pagebufferguide.h"
#include "page/pagekeypad.h"
#include "utility.h"
#include "pagereturn.h"

PageBufferGuide::PageBufferGuide(Action action, RestInfoData *userInfo, PageManager *stateManager, QWidget *parent) :
    PageWithUserBar(UserStatusBarWidget::leaveAndOk, stateManager, userInfo, parent),
    action(action),
    targetAccount(),
    amount(-1),
    userInfo(userInfo),
    ui(new Ui::PageBufferGuide)
{
    ui->setupUi(this);
    this->setupUserBar(this->ui->widgetRootLayout);

    if(action == Action::Deposit) {
        this->setWindowTitle(QCoreApplication::translate("PageDeposit", "Talletus", nullptr));
        this->ui->lblInfo->setText(QCoreApplication::translate("PageDeposit", "Seuraavassa ruudussa voit sy\303\266tt\303\244\303\244 telletettavan summan", nullptr));
    } else {
        this->setWindowTitle(QCoreApplication::translate("PageDeposit", "Tilisiirto", nullptr));
        this->ui->lblInfo->setText(QCoreApplication::translate("PageDeposit", "Seuraavassa kahdessa ruudussa\nvoit sy\303\266tt\303\244\303\244 tilin numeron ja siirettävän summan", nullptr));
    }
}

void PageBufferGuide::onOk() {
    if(this->action == Action::Transfer) {
        this->navigate<PageKeypad>(PageKeypad::AccountNumber, this->userInfo);
    } else {
        this->navigate<PageKeypad>(PageKeypad::Deposit, this->userInfo);
    }
}

QVariant PageBufferGuide::onNaviagte(const QMetaObject *oldPage, bool closed, QVariant *result) {
    if(closed) {
        // If result is bool it will always be false (User canceled action)
        if(
               !oldPage->inherits(&PageKeypad::staticMetaObject) ||
               result->type() == QVariant::Bool ||
               result->type() != QVariant::Double
          ) {
            return QVariant::fromValue(PageManager::Leave);
        }

        if(this->action == Action::Deposit) {
            this->amount = result->toDouble();
            this->pageManager->getRESTInterface(false)->deposit(this->amount);
            return QVariant::fromValue(PageManager::KeepLoading);
        } else if(this->targetAccount.isNull()) {
            this->targetAccount = QString::number(result->toDouble(), 'f', 0);
            return QVariant::fromValue(new PageReturn(new PageKeypad(PageKeypad::AccountSum, this->userInfo, this->pageManager), PageReturn::AddOnTop));
        } else {
            this->amount = result->toDouble();
            this->pageManager->getRESTInterface(false)->transfer(this->targetAccount, this->amount);
            return QVariant::fromValue(PageManager::KeepLoading);
        }
    }

    return QVariant::fromValue(PageManager::Stay);
}

PageBase::RestDataAction PageBufferGuide::onRestData(RestReturnData *data) {
    if(data->type() != RestReturnData::typeDeposit && data->type() != RestReturnData::typeTransfer) {
        return RestDataAction::Skip;
    }

    if(this->handleRestError(data, tr(action == Action::Deposit ? "talletettaessa" : "siirtäessä"))) {
        return RestDataAction::Delete;
    }

    this->pageManager->leaveCurrentPage(
        QVariant::fromValue(new PageReturn(
            this->pageManager->createPrompt(
                tr(this->action == Action::Deposit ? "Talletus onnistui" : "Siirot onnistui"),
                this->action == Action::Deposit ?
                    tr("Talletit %1€").arg(QString::number(this->amount, 'f', 2)) :
                    tr("Siirsit %1€ tilille %2").arg(QString::number(this->amount, 'f', 2), this->targetAccount),
                PromptEnum::info, 0
            ), PageReturn::LeaveCurrent
        ))
    );

    return RestDataAction::Delete;
}

PageBufferGuide::~PageBufferGuide() {
    delete ui;
}
