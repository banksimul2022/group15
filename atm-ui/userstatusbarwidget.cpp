#include "userstatusbarwidget.h"
#include "ui_userstatusbarwidget.h"

UserStatusBarWidget::UserStatusBarWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserStatusBarWidget)
{
    ui->setupUi(this);
}

UserStatusBarWidget::~UserStatusBarWidget() {
    delete ui;
}
