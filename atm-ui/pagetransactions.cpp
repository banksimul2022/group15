#include "pagetransactions.h"
#include "ui_pagetransactions.h"
#include "userstatusbarwidget.h"

PageTransactions::PageTransactions(StateManager *stateManager, QWidget *parent) :
    PageWithUserBar(UserStatusBarWidget::Mode::leaveOnly, stateManager, parent),
    ui(new Ui::PageTransactions)
{
    ui->setupUi(this);
    this->setupUserBar(this->ui->widgetRootLayout);
}

PageTransactions::~PageTransactions() {
    delete ui;
}
