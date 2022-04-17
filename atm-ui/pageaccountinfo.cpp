#include "pageaccountinfo.h"
#include "ui_pageaccountinfo.h"
#include "userstatusbarwidget.h"

PageAccountInfo::PageAccountInfo(StateManager *stateManager, QWidget *parent) :
    PageWithUserBar(UserStatusBarWidget::Mode::leaveOnly, stateManager, parent),
    ui(new Ui::PageAccountInfo)
{
    ui->setupUi(this);
    this->setupUserBar(this->ui->widgetRootLayout);
}

PageAccountInfo::~PageAccountInfo() {
    delete ui;
}
