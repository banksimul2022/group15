#include "page/pagemainaccountview.h"
#include "ui_pagemainaccountview.h"
#include "userstatusbarwidget.h"

#include "page/pageaccountinfo.h"
#include <page/pagewithdraw.h>

PageMainAccountView::PageMainAccountView(StateManager *stateManager, QWidget *parent) :
    PageWithUserBar(UserStatusBarWidget::Mode::logout, stateManager, nullptr, parent),
    userInfo(nullptr),
    ui(new Ui::PageMainAccountView)
{
    ui->setupUi(this);
    this->setupUserBar(this->ui->widgetRootLayout);
    this->stateManager->getRESTInterface()->getInfo();
}

PageMainAccountView::~PageMainAccountView() {
    if(this->userInfo != nullptr) delete this->userInfo;
    delete ui;
}

bool PageMainAccountView::keepLoadingPageOnNavigate() { return true; }

void PageMainAccountView::onRestData(RestReturnData *data) {
    PageWithUserBar::onRestData(data); // Call parent data processor

    if(data->type() != RestReturnData::typeInfo) {
        return;
    }

    if(data->error() != -1) {
        qDebug() << "ERROR getting user info..." << data->error();
        delete data;
        this->stateManager->leaveAllPages(QVariant());
        return;
    }

    this->userInfo = static_cast<RestInfoData*>(data);
    this->ui->lblAccountInfo->setText(this->ui->lblAccountInfo->text().arg(this->userInfo->getfName(), this->userInfo->getlName(), this->userInfo->getAccountNumber()));
    this->stateManager->leaveLoadingPage();
}

void PageMainAccountView::on_btnWidthdraw_clicked() {
    this->navigate<PageWithdraw>(this->userInfo);
}

void PageMainAccountView::on_btnBalance_clicked() {
    this->navigate<PageAccountInfo>(PageAccountInfo::ViewBalance, this->userInfo);
}
