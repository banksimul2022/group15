#include "page/pagemainaccountview.h"
#include "ui_pagemainaccountview.h"
#include "userstatusbarwidget.h"

PageMainAccountView::PageMainAccountView(StateManager *stateManager, QWidget *parent) :
    PageWithUserBar(UserStatusBarWidget::Mode::logout, stateManager, nullptr, parent),
    ui(new Ui::PageMainAccountView)
{
    ui->setupUi(this);
    this->setupUserBar(this->ui->widgetRootLayout);
    this->stateManager->getRESTInterface()->getInfo();
}

PageMainAccountView::~PageMainAccountView() {
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

    RestInfoData *userInfo = static_cast<RestInfoData*>(data);
    this->ui->lblAccountInfo->setText(this->ui->lblAccountInfo->text().arg(userInfo->getfName(), userInfo->getlName(), userInfo->getAccountNumber()));
    delete data;
    this->stateManager->leaveLoadingPage();
}
