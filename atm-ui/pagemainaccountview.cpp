#include "pagemainaccountview.h"
#include "ui_pagemainaccountview.h"
#include "userstatusbarwidget.h"

PageMainAccountView::PageMainAccountView(StateManager *stateManager, QWidget *parent) :
    PageWithUserBar(stateManager, parent),
    ui(new Ui::PageMainAccountView)
{
    ui->setupUi(this);
    this->setupUserBar(this->ui->widgetRootLayout);
}

PageMainAccountView::~PageMainAccountView() {
    delete ui;
}
