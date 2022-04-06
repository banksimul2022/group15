#include "pagemainaccountview.h"
#include "ui_pagemainaccountview.h"

PageMainAccountView::PageMainAccountView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageMainAccountView)
{
    ui->setupUi(this);
}

PageMainAccountView::~PageMainAccountView() {
    delete ui;
}
