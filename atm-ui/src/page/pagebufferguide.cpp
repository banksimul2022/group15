#include "page/pagebufferguide.h"
#include "ui_pagebufferguide.h"
#include "page/dialog/pagekeypad.h"
#include "utility.h"
#include "pagereturn.h"

#include <resterrorcode.h>

PageBufferGuide::PageBufferGuide(Action action, RestInfoData *userInfo, PageManager *pageManager, QWidget *parent) :
    PageWithUserBar(UserStatusBarWidget::leaveAndOk, pageManager, userInfo, parent),
    action(action),
    inputStr1(),
    inputStr2(),
    amount(-1),
    userInfo(userInfo),
    ui(new Ui::PageBufferGuide)
{
    ui->setupUi(this);
    this->setupUserBar(this->ui->widgetRootLayout);

    switch (action) {
        case Action::Deposit:
            this->setWindowTitle(QCoreApplication::translate("PageDeposit", "Talletus", nullptr));
            this->ui->lblInfo->setText(QCoreApplication::translate("PageDeposit", "Seuraavassa ruudussa voit sy\303\266tt\303\244\303\244 telletettavan summan", nullptr));
            break;

        case Action::Transfer:
            this->setWindowTitle(QCoreApplication::translate("PageDeposit", "Tilisiirto", nullptr));
            this->ui->lblInfo->setText(QCoreApplication::translate("PageDeposit", "Seuraavassa kahdessa ruudussa\nvoit sy\303\266tt\303\244\303\244 tilin numeron ja siirettävän summan", nullptr));
            break;

        case Action::ChangePIN:
            this->setWindowTitle(QCoreApplication::translate("PageDeposit", "PIN Vaihto", nullptr));
            this->ui->lblInfo->setText(QCoreApplication::translate("PageDeposit", "Seuraavassa kolmessa ruudussa syötät nykyisen PIN koodisi ja uuden PIN koodin kaksi kertaa", nullptr));
            break;
    }
}

void PageBufferGuide::onOk() {
    switch (this->action) {
        case Action::Deposit:
            this->navigate<PageKeypad>(false, tr("Tilisiirto - Tilinumero"), tr("Tilinumero:"), this->userInfo);
            break;

        case Action::Transfer:
            this->navigate<PageKeypad>(true, tr("Talletus - Summan syöttö"), tr("Talletettava summa:"), this->userInfo);
            break;

        case Action::ChangePIN:
            this->navigate<PageKeypad>(true, tr("PIN Vaihto - Nykyinen PIN"), tr("Nykyinen PIN:"), this->userInfo);
            break;
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

        switch(this->action) {
            case Action::Deposit:
                this->amount = result->toDouble();
                this->pageManager->getRESTInterface(false)->deposit(this->amount);
                return QVariant::fromValue(PageManager::KeepLoading);

            case Action::Transfer:
                if(this->inputStr1.isNull()) {
                    this->inputStr1 = QString::number(result->toDouble(), 'f', 0); // inputStr1 = targetAccount
                    return QVariant::fromValue(new PageReturn(new PageKeypad(true, tr("Tilisiirto - Summa"), tr("Siirrettävä summa:"), this->userInfo, this->pageManager), PageReturn::AddOnTop));
                }

                this->amount = result->toDouble();
                this->pageManager->getRESTInterface(false)->transfer(this->inputStr1, this->amount); // inputStr1 = targetAccount
                return QVariant::fromValue(PageManager::KeepLoading);

            case Action::ChangePIN:
                if(this->inputStr1.isNull()) { // inputStr1 = currentPin
                    this->inputStr1 = QString::number(result->toDouble(), 'f', 0);
                    return QVariant::fromValue(new PageReturn(new PageKeypad(true, tr("PIN Vaihto - Uusi PIN"), tr("Uusi PIN:"), this->userInfo, this->pageManager), PageReturn::AddOnTop));
                } else if(this->inputStr2.isNull()) { // inputStr2 = newPin
                    this->inputStr2 = QString::number(result->toDouble(), 'f', 0);
                    return QVariant::fromValue(new PageReturn(new PageKeypad(true, tr("PIN Vaihto - Uusi PIN"), tr("Uusi PIN uudelleen:"), this->userInfo, this->pageManager), PageReturn::AddOnTop));
                } else if(this->inputStr2 != QString::number(result->toDouble(), 'f', 0)) { // check that inputStr2 (newPin) equals the second new pin input round
                    return QVariant::fromValue(
                                new PageReturn(
                                    this->pageManager->createPrompt(
                                        tr("PIN Vaihto - PIN ei täsmää"), tr("Syöttämäsi uudet PIN koodit eivät täsmänneet!\nYritä uudelleen..."),
                                        PromptEnum::warning, 0
                                    ), PageReturn::LeaveCurrent
                                )
                           );
                }

                this->pageManager->getRESTInterface()->changePin(this->inputStr1, this->inputStr2); // inputStr1 = currentPin; inputStr2 = newPin
                return QVariant::fromValue(PageManager::KeepLoading);
        }
    }

    return QVariant::fromValue(PageManager::Stay);
}

PageBase::RestDataAction PageBufferGuide::onRestData(RestReturnData *data) {
    if(
           data->type() != RestReturnData::typeDeposit  &&
           data->type() != RestReturnData::typeTransfer &&
           data->type() != RestReturnData::typePinChange
      ) {
        return RestDataAction::Skip;
    }

    switch(data->error()) {
        case RestErrors::ERR_UNKNOWN_ACCOUNT:
            this->pageManager->displayPrompt(
                tr("Tunematon tili"),
                tr("Antamallasi tilinumerolla ei löytynyt tiliä"),
                PromptEnum::warning, 0
            );
            return RestDataAction::Delete;

        case RestErrors::ERR_INSUFFICIENT_FUNDS:
            this->pageManager->displayPrompt(
                tr("Tilillä ei ole katetta"),
                tr("Tililläsi ei ole tarpeeksi katetta (%1€) tilisiirtoon").arg(QString::number(this->amount, 'f', 2)),
                PromptEnum::warning, 0
            );
            return RestDataAction::Delete;

        case RestErrors::ERR_NEW_PIN_INVALID:
            this->pageManager->displayPrompt(
                tr("Uusi PIN virheelinen"),
                tr("Antamasi uusi PIN on virheelinen! (Pitää olla 4 numeroa)"),
                PromptEnum::warning, 0
            );
            return RestDataAction::Delete;

        case RestErrors::ERR_SAME_PIN:
            this->pageManager->displayPrompt(
                tr("Uusi PIN sama"),
                tr("Antamasi PIN on sama kuin vanha"),
                PromptEnum::warning, 0
            );
            return RestDataAction::Delete;

        default: {
            const char *failedAction = nullptr;

            switch(this->action) {
                case Action::Deposit:
                    failedAction = "talletettaessa";
                    break;
                case Action::Transfer:
                    failedAction = "siirtäessä";
                    break;
                case Action::ChangePIN:
                    failedAction = "vaihtaessa PIN";
                    break;
            }

            if(this->handleRestError(data, tr(failedAction))) {
                return RestDataAction::Delete;
            }

            break;
        }
    }

    const char *title = nullptr;
    QString message = nullptr;

    switch(this->action) {
        case Action::Deposit:
            title = "Talletus onnistui";
            message = tr("Talletit %1€").arg(QString::number(this->amount, 'f', 2));
            break;
        case Action::Transfer:
            title = "Siirto onnistui";
            message = tr("Siirsit %1€ tilille %2").arg(QString::number(this->amount, 'f', 2), this->inputStr1); // inputStr1 = targetAccount
            break;
        case Action::ChangePIN:
            title = "PIN vaihto onnistui";
            message = tr("PIN koodisi vaihdettiin onnistuneesti");
            break;
    }

    this->pageManager->leaveCurrentPage(
        QVariant::fromValue(new PageReturn(this->pageManager->createPrompt(tr(title), message, PromptEnum::info, 0), PageReturn::LeaveCurrent))
    );

    return RestDataAction::Delete;
}

PageBufferGuide::~PageBufferGuide() {
    delete ui;
}
