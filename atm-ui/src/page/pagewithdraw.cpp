#include "page/pagewithdraw.h"
#include "ui_pagewithdraw.h"
#include "userstatusbarwidget.h"
#include "page/pagekeypad.h"
#include "page/pageprompt.h"
#include "utility.h"

#include <resterrorcode.h>
#include <QDebug>

PageWithdraw::PageWithdraw(RestInfoData *userInfo, StateManager *stateManager, QWidget *parent) :
    PageWithUserBar(UserStatusBarWidget::leaveOnly, stateManager, userInfo, parent),
    useCredit(false),
    amountWithdrawn(0),
    ui(new Ui::PageWithdraw)
{
    ui->setupUi(this);
    this->setupUserBar(this->ui->widgetRootLayout);

    QList<QPushButton*> withdrawButtons = this->findChildren<QPushButton*>();

    foreach(QPushButton *btn, withdrawButtons) {
        if(btn->parent() == this) {
            connect(btn, &QPushButton::clicked, this, &PageWithdraw::onAmountButtonPress);
        }
    }

    this->userInfo = userInfo;
}

QVariant PageWithdraw::onNaviagte(const QMetaObject *oldPage, bool closed, QVariant *result) {
    if(!closed) {
        return QVariant::fromValue(StateManager::Stay);
    }

    if(oldPage->inherits(&PageKeypad::staticMetaObject)) {
        if(result->type() == QVariant::Double) {
            this->amountWithdrawn = result->toDouble();
            this->stateManager->getRESTInterface(false)->withdraw(this->amountWithdrawn, this->useCredit);
        } else { // result will be a bool on user cancel (always false) and unexpected return values also are a reason to leave
            return QVariant::fromValue(StateManager::Leave);
        }

        return QVariant::fromValue(StateManager::KeepLoading);
    } else if(oldPage->inherits(&PagePrompt::staticMetaObject)) {
        if(result->type() == QVariant::Int) {
            this->useCredit = result->toInt(); // Store the user answer in useCredit
            return QVariant::fromValue(StateManager::Stay);
        }
    }

    return QVariant::fromValue(StateManager::Leave);
}

void PageWithdraw::onReady() {
    if(this->userInfo->credit()) {
        QString debit = tr("Debit"), credit = tr("Credit");
        this->stateManager->displayPrompt(
            tr("Valitse nostotapa"),
            tr("Haluatko käyttää credit vai debit korttia?"),
            PromptEnum::question,
            3, nullptr, &debit, &credit
        );
    }
}

void PageWithdraw::onRestData(RestReturnData *data) {
    if(data->type() != RestReturnData::typeWithdraw) {
        return;
    }

    QWidget *prompt = nullptr;

    if(data->error() == RestErrors::ERR_INSUFFICIENT_FUNDS) {
        prompt = this->stateManager->createPrompt(
                     tr("Virhe"),
                     tr("Tililläsi ei ole katetta %1€ nostamiseen!").arg(this->amountWithdrawn),
                     PromptEnum::warning,
                     0
                );
    } else if(this->handleRestError(data, tr("nostamisessa"))) {
        return;
    } else {
        prompt = this->stateManager->createPrompt(
                     tr("Nosto onnistui"),
                     (
                         this->userInfo->credit() ?
                         tr(this->useCredit ? "Nostettu %1€ credittiä" : "Nostettu %1€ debittiä").arg(this->amountWithdrawn) :
                         tr("Nostettu %1€").arg(this->amountWithdrawn)
                     ),
                     PromptEnum::info,
                     0
                );
    }

    delete data;
    this->stateManager->leaveCurrentPage(QVariant::fromValue(prompt));
}

void PageWithdraw::onAmountButtonPress() {
    QString name = this->sender()->objectName();

    if(name.startsWith("btnAmount")) {
        bool ok;
        this->amountWithdrawn = name.remove(0, 9).toDouble(&ok);
        Q_ASSERT(ok);
        this->stateManager->getRESTInterface()->withdraw(this->amountWithdrawn, this->useCredit);
    } else if(name == "btnOther") {
        this->navigate<PageKeypad>(PageKeypad::Withdraw, this->userInfo);
    } else {
        // Code execution SHOULD not get here
        Q_ASSERT(false);
    }
}

PageWithdraw::~PageWithdraw() {
    delete ui;
}
