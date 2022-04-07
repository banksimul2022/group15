#include "pagetransactions.h"
#include "ui_pagetransactions.h"
#include "userstatusbarwidget.h"

PageTransactions::PageTransactions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageTransactions)
{
    ui->setupUi(this);
    this->ui->widgetRootLayout->addWidget(new UserStatusBarWidget);
}

PageTransactions::~PageTransactions() {
    delete ui;
}
