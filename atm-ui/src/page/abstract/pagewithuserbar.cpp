#include "page/abstract/pagewithuserbar.h"

#include <QDebug>

PageWithUserBar::PageWithUserBar(UserStatusBarWidget::Mode mode, PageManager *stateManager, RestInfoData *infoData, QWidget *parent) : PageBase{stateManager, parent} {
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

PageBase::RestDataAction PageWithUserBar::onRestData(RestReturnData *data) {
    if(this->userStatusBar->mode() != UserStatusBarWidget::Mode::logout || data->type() != RestReturnData::typeLogout) {
        return RestDataAction::Skip;
    }

    if(this->handleRestError(data, tr("ulos kirjautuessa"))) {
        return RestDataAction::Delete;
    }

    this->pageManager->leaveAllPages(QVariant());
    return RestDataAction::Delete;
}

void PageWithUserBar::onExtraButton(int id) { Q_UNUSED(id) }

void PageWithUserBar::onLeave() {
    if(this->userStatusBar->mode() == UserStatusBarWidget::Mode::logout) {
        this->pageManager->getRESTInterface()->logout();
    } else {
        this->pageManager->leaveCurrentPage(QVariant::fromValue(PageManager::Leave));
    }
}

void PageWithUserBar::onOk() { /* Not used in base class */ }

PageWithUserBar::~PageWithUserBar() {

}
