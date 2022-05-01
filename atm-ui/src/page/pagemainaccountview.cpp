#include "page/pagemainaccountview.h"
#include "ui_pagemainaccountview.h"
#include "userstatusbarwidget.h"
#include "page/pageaccountinfo.h"
#include "page/pagewithdraw.h"
#include "page/pagebufferguide.h"

PageMainAccountView::PageMainAccountView(PageManager *stateManager, QWidget *parent) :
    PageWithUserBar(UserStatusBarWidget::Mode::logout, stateManager, nullptr, parent),
    userInfo(nullptr),
    ui(new Ui::PageMainAccountView)
{
    ui->setupUi(this);
    this->setupUserBar(this->ui->widgetRootLayout);
}

PageMainAccountView::~PageMainAccountView() {
    if(this->userInfo != nullptr) delete this->userInfo;
    delete ui;
}

QVariant PageMainAccountView::onNaviagte(const QMetaObject *oldPage, bool closed, QVariant *result) {
    Q_UNUSED(oldPage) Q_UNUSED(result)
    return closed ? QVariant::fromValue(PageManager::Stay) : QVariant::fromValue(PageManager::KeepLoading);
}

void PageMainAccountView::onReady() {
    this->pageManager->getRESTInterface()->getInfo();
}

PageBase::RestDataAction PageMainAccountView::onRestData(RestReturnData *data) {
    PageWithUserBar::onRestData(data); // Call parent data processor

    if(data->type() != RestReturnData::typeInfo) {
        return RestDataAction::Skip;
    }

    if(this->handleRestError(data, tr("haettaessa käyttäjän tietoja"))) {
        this->pageManager->leaveAllPages(QVariant());
        return RestDataAction::Delete;
    }

    this->userInfo = static_cast<RestInfoData*>(data);
    this->ui->lblAccountInfo->setText(this->ui->lblAccountInfo->text().arg(this->userInfo->getfName(), this->userInfo->getlName(), this->userInfo->getAccountNumber()));
    this->ui->lblProfilePicture->setPixmap(this->userInfo->getProfile().scaled(this->ui->lblProfilePicture->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    this->pageManager->leaveLoadingPage();

    return RestDataAction::SetNull;
}

void PageMainAccountView::on_btnWidthdraw_clicked() {
    this->navigate<PageWithdraw>(this->userInfo);
}

void PageMainAccountView::on_btnBalance_clicked() {
    this->navigate<PageAccountInfo>(PageAccountInfo::ViewBalance, this->userInfo);
}

void PageMainAccountView::on_btnTransactions_clicked() {
    this->navigate<PageAccountInfo>(PageAccountInfo::ViewTransactions, this->userInfo);
}

void PageMainAccountView::on_btnDeposit_clicked() {
    this->navigate<PageBufferGuide>(PageBufferGuide::Deposit, this->userInfo);
}

void PageMainAccountView::on_btnTransfer_clicked() {
    this->navigate<PageBufferGuide>(PageBufferGuide::Transfer, this->userInfo);
}

void PageMainAccountView::on_btnChangePin_clicked() {
    this->navigate<PageBufferGuide>(PageBufferGuide::ChangePIN, this->userInfo);
}

