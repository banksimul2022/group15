#include "page/abstract/pagewithuserbar.h"

#include <QDebug>

PageWithUserBar::PageWithUserBar(UserStatusBarWidget::Mode mode, StateManager *stateManager, RestInfoData *infoData, QWidget *parent) : PageBase{stateManager, parent} {
    this->userStatusBar = new UserStatusBarWidget(mode, infoData, this);
    this->connect(this->userStatusBar, &UserStatusBarWidget::extraButton, this, &PageWithUserBar::onExtraButton);
    this->connect(this->userStatusBar, &UserStatusBarWidget::leave, this, &PageWithUserBar::onLeave);

    if(mode == UserStatusBarWidget::Mode::leaveAndOk) {
        this->connect(this->userStatusBar, &UserStatusBarWidget::ok, this, &PageWithUserBar::onOk);
    }
}

void PageWithUserBar::stopTimer() {
    this->userStatusBar->stopLeaveTimeout();
}

void PageWithUserBar::startTimer() {
    this->userStatusBar->resetLeaveTimeout();
}

void PageWithUserBar::setupUserBar(QLayout *layout) {
    layout->addWidget(this->userStatusBar);
}

void PageWithUserBar::onRestData(RestReturnData *data) {
    PageBase::onRestData(data); // Call parent class method

    if(this->userStatusBar->mode() != UserStatusBarWidget::Mode::logout || data->type() != RestReturnData::typeLogout) {
        return;
    }

    int error = data->error();
    delete data;

    if(error != -1) {
        //TODO: Replace with prompt
        qDebug() << "ERROR during logout:" << error;
    }

    this->stateManager->leaveAllPages(QVariant());
}

void PageWithUserBar::onExtraButton(int id) { Q_UNUSED(id) }

void PageWithUserBar::onLeave() {
    if(this->userStatusBar->mode() == UserStatusBarWidget::Mode::logout) {
        this->stateManager->getRESTInterface()->logout();
    } else {
        this->stateManager->leaveCurrentPage(QVariant());
    }
}

void PageWithUserBar::onOk() { /* Not used in base class */ }

PageWithUserBar::~PageWithUserBar() {

}
