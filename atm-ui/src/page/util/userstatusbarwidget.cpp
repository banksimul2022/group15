#include "page/util/userstatusbarwidget.h"
#include "ui_userstatusbarwidget.h"
#include "utility.h"

#include <QSignalMapper>
#include <QPushButton>
#include <QTimer>

UserStatusBarWidget::UserStatusBarWidget(Mode mode, RestInfoData *userInfo, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserStatusBarWidget),
    extraBtnMapper(nullptr),
    barMode(mode),
    leaveTimout(this->getDefaultTimeout())
{
    ui->setupUi(this);
    this->updateTimeoutLabel();

    if(userInfo != nullptr) {
        this->ui->lblUserInfo->setText(
            QCoreApplication::translate("UserStatusBarWidget", "%1 %2 Tili %3", nullptr)
                .arg(userInfo->getfName(), userInfo->getlName(), userInfo->getAccountNumber())
        );
    } else {
        this->ui->lblUserInfo->setVisible(false);
    }

    Utility::retainSizeWhenHidden(this->ui->btnLeave);
    Utility::retainSizeWhenHidden(this->ui->btnOk);

    this->connect(this->ui->btnLeave, &QPushButton::clicked, this, &UserStatusBarWidget::leave);
    this->connect(&this->leaveTimer, &QTimer::timeout, this, &UserStatusBarWidget::leaveTimerTick);

    this->leaveTimer.setInterval(1000);

    if(barMode == Mode::logout) {
        this->ui->btnLeave->setText(QCoreApplication::translate("UserStatusBarWidget", "Kirjaudu Ulos", nullptr));
        this->ui->lblTimerInfo->setText(QCoreApplication::translate("UserStatusBarWidget", "Uloskirjautumis ajastin:", nullptr));
    } else {
        this->ui->btnLeave->setText(QCoreApplication::translate("UserStatusBarWidget", "Takaisin", nullptr));
        this->ui->lblTimerInfo->setText(QCoreApplication::translate("UserStatusBarWidget", "Palataan takasin:", nullptr));
    }

    if(mode != Mode::custom) {
        if(barMode == Mode::logout || barMode == Mode::leaveOnly) {
            this->ui->btnOk->setVisible(false);
            this->ui->buttonsLayout->setDirection(QBoxLayout::RightToLeft);
        } else {
            this->connect(this->ui->btnOk, &QPushButton::clicked, this, &UserStatusBarWidget::ok);
        }
    }
}

void UserStatusBarWidget::setButtonTitles(int count, ...) {
    va_list args;
    va_start(args, count);
    this->setButtonTitles(count, args);
    va_end(args);
}

void UserStatusBarWidget::setButtonTitles(int count, va_list args) {
    if(this->barMode != Mode::custom) {
        return;
    }

    if(count-- > 0) {
        QString *text = va_arg(args, QString*);
        if(text != nullptr) {
            this->ui->btnLeave->setText(*text);
        }
    }

    if(count-- > 0) {
        if(this->extraBtnMapper == nullptr) {
            this->extraBtnMapper = new QSignalMapper(this); // Setting the parent automatically deletes the object when the parent is deleted
            this->connect(this->extraBtnMapper, &QSignalMapper::mappedInt, this, &UserStatusBarWidget::extraButton);
        }

        this->extraBtnMapper->setMapping(this->ui->btnOk, 0);
        this->connect(this->ui->btnOk, &QPushButton::clicked, extraBtnMapper, QOverload<>::of(&QSignalMapper::map));
        this->ui->btnOk->setText(*va_arg(args, QString*));
    } else {
        this->ui->btnOk->setVisible(false);
        this->ui->buttonsLayout->setDirection(QBoxLayout::RightToLeft);
        return;
    }

    QFont font = this->ui->btnLeave->font();

    for(int i = 0; i < count; i++) {
        QPushButton *btn = new QPushButton(this);
        btn->setObjectName(QString("btnExtra%1").arg(i));
        btn->setMinimumSize(QSize(50, 50));
        btn->setFont(font);
        btn->setText(*va_arg(args, QString*));
        this->extraBtnMapper->setMapping(btn, i + 1);
        this->connect(btn, &QPushButton::clicked, extraBtnMapper, QOverload<>::of(&QSignalMapper::map));
        this->ui->buttonsLayout->addWidget(btn);
    }
}

void UserStatusBarWidget::stopLeaveTimeout() {
    this->leaveTimer.stop();
}

void UserStatusBarWidget::startLeaveTimeout() {
    this->leaveTimer.start();
}

void UserStatusBarWidget::resetLeaveTimeout(bool start) {
    if(start && !this->leaveTimer.isActive()) {
        this->leaveTimer.start();
    }

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
        this->leaveTimer.stop();
        emit leave();
    }

    this->updateTimeoutLabel();
}

uint UserStatusBarWidget::getDefaultTimeout() {
    return this->barMode == Mode::logout ? UserStatusBarWidget::defaultLogoutTimeout : UserStatusBarWidget::defaultLeaveTimeout;
}

UserStatusBarWidget::~UserStatusBarWidget() {
    if(this->leaveTimer.isActive()) {
        this->leaveTimer.stop();
    }

    delete ui;
}

