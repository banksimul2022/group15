#include "pagemainaccountview.h"
#include "ui_pagemainaccountview.h"
#include "userstatusbarwidget.h"

PageMainAccountView::PageMainAccountView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageMainAccountView)
{
    ui->setupUi(this);
    this->ui->widgetRootLayout->addWidget(new UserStatusBarWidget);
}

PageMainAccountView::~PageMainAccountView() {
    delete ui;
}
