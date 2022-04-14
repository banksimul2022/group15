#include "userstatusbarwidget.h"
#include "ui_userstatusbarwidget.h"
#include "utility.h"

#include <QSignalMapper>
#include <QTimer>

UserStatusBarWidget::UserStatusBarWidget(Mode mode, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserStatusBarWidget),
    barMode(mode),
    leaveTimer(new QTimer(this)),
    leaveTimout(this->getDefaultTimeout())
{
    ui->setupUi(this);
    this->updateTimeoutLabel();

    Utility::retainSizeWhenHidden(this->ui->btnLeave);
    Utility::retainSizeWhenHidden(this->ui->btnOk);

    this->connect(this->ui->btnLeave, &QPushButton::clicked, this, &UserStatusBarWidget::leave);

    if(barMode == Mode::logout || barMode == Mode::leaveOnly) {
        this->ui->btnOk->setVisible(false);
        this->ui->buttonsLayout->setDirection(QBoxLayout::RightToLeft);
    } else {
        this->connect(this->ui->btnOk, &QPushButton::clicked, this, &UserStatusBarWidget::ok);
    }

    if(barMode == Mode::logout) {
        this->ui->btnLeave->setText(QCoreApplication::translate("UserStatusBarWidget", "Kirjaudu Ulos", nullptr));
        this->ui->lblTimerInfo->setText(QCoreApplication::translate("UserStatusBarWidget", "Uloskirjautumis ajastin:", nullptr));
    } else {
        this->ui->btnLeave->setText(QCoreApplication::translate("UserStatusBarWidget", "Takaisin", nullptr));
        this->ui->lblTimerInfo->setText(QCoreApplication::translate("UserStatusBarWidget", "Palataan takasin:", nullptr));
    }

    this->connect(this->leaveTimer, &QTimer::timeout, this, &UserStatusBarWidget::leaveTimerTick);
    this->leaveTimer->start(1000);
}

void UserStatusBarWidget::resetLeaveTimeout() {
    this->leaveTimout = this->getDefaultTimeout();
    this->updateTimeoutLabel();
}

UserStatusBarWidget::Mode UserStatusBarWidget::mode() {
    return this->barMode;
}

void UserStatusBarWidget::updateTimeoutLabel() {
    this->ui->lblTimer->setText(QString::number(this->leaveTimout));
}

void UserStatusBarWidget::leaveTimerTick() {
    if(--this->leaveTimout < 1) {
        this->leaveTimer->stop();
        emit leave();
    }

    this->updateTimeoutLabel();
}

uint UserStatusBarWidget::getDefaultTimeout() {
    return this->barMode == Mode::logout ? UserStatusBarWidget::defaultLogoutTimeout : UserStatusBarWidget::defaultLeaveTimeout;
}

UserStatusBarWidget::~UserStatusBarWidget() {
    if(this->leaveTimer->isActive()) {
        this->leaveTimer->stop();
    }

    delete ui;
}

