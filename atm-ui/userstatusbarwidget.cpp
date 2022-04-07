#include "userstatusbarwidget.h"
#include "ui_userstatusbarwidget.h"

#include <QTimer>

UserStatusBarWidget::UserStatusBarWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserStatusBarWidget),
    logoutTimer(new QTimer),
    logoutTimout(UserStatusBarWidget::defaultLogoutTimeout)
{
    ui->setupUi(this);
    this->updateTimeoutLabel();

    this->btnConnection = connect(this->ui->btnLogout, &QPushButton::clicked, this, &UserStatusBarWidget::logout);
    connect(this->logoutTimer, &QTimer::timeout, this, &UserStatusBarWidget::logoutTimerTick);

    this->logoutTimer->start(1000);
}

void UserStatusBarWidget::resetTimeout() {
    this->logoutTimout = UserStatusBarWidget::defaultLogoutTimeout;
    this->updateTimeoutLabel();
}

void UserStatusBarWidget::updateTimeoutLabel() {
    this->ui->lblTimer->setText(QString::number(this->logoutTimout));
}

void UserStatusBarWidget::logoutTimerTick() {
    if(--this->logoutTimout < 1) {
        this->logoutTimer->stop();

        // Prevent calling the logout signal twice by disconnecting the button on timeout
        disconnect(this->btnConnection);

        emit logout();
    }

    this->updateTimeoutLabel();
}

UserStatusBarWidget::~UserStatusBarWidget() {
    if(this->logoutTimer->isActive()) {
        this->logoutTimer->stop();
    }

    delete this->logoutTimer;
    delete ui;
}
