#include "page/pagewithdraw.h"
#include "ui_pagewithdraw.h"
#include "userstatusbarwidget.h"
#include "page/pagekeypad.h"
#include "page/pageprompt.h"
#include "utility.h"

#include <QDebug>

PageWithdraw::PageWithdraw(RestInfoData *userInfo, StateManager *stateManager, QWidget *parent) :
    PageWithUserBar(UserStatusBarWidget::leaveOnly, stateManager, userInfo, parent),
    useCredit(false),
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

void PageWithdraw::onNavigate() {
    if(this->userInfo->credit()) {
        this->stateManager->displayPrompt(this, "Valitse nosto tapa", "Haluatko käyttää credit vai debit korttia?", PromptEnum::question, 3, nullptr, "Debit", "Credit");
    }
}

bool PageWithdraw::processResult(QWidget *page, QVariant result) {
    if(Utility::isOfType<PageKeypad>(page)) {
        if(result.type() == QVariant::Double) {
            this->stateManager->getRESTInterface()->withdraw(result.toDouble(), this->useCredit);
        } else if(result.type() != QVariant::Bool) {
            return true;
        }

        return false;
    } else if(Utility::isOfType<PagePrompt>(page)) {
        if(result.type() == QVariant::Int) {
            this->useCredit = result.toInt(); // Store the user answer in useCredit
            return false;
        }
    }

    return true;
}

void PageWithdraw::onRestData(RestReturnData *data) {
    if(data->type() != RestReturnData::typeWithdraw) {
        return;
    }

    if(data->error() != -1) {
        // TODO: Replace with error dialog
        qDebug() << "ERROR witdrawing money" << data->error();
    }

    delete data;

    // TODO: Replace with notification
    this->stateManager->leaveCurrentPage(QVariant());
}

void PageWithdraw::onAmountButtonPress() {
    QString name = this->sender()->objectName();

    if(name.startsWith("btnAmount")) {
        bool ok;
        uint val = name.remove(0, 9).toUInt(&ok);
        Q_ASSERT(ok);
        this->stateManager->getRESTInterface()->withdraw(val, this->useCredit);
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
