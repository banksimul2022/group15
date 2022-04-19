#include "pagewithuserbar.h"

#include <QDebug>

PageWithUserBar::PageWithUserBar(UserStatusBarWidget::Mode mode, StateManager *stateManager, QWidget *parent) : PageBase{stateManager, parent} {
    this->userStatusBar = new UserStatusBarWidget(mode, this);
    this->connect(this->userStatusBar, &UserStatusBarWidget::extraButton, this, &PageWithUserBar::onExtraButton);
    this->connect(this->userStatusBar, &UserStatusBarWidget::leave, this, &PageWithUserBar::onLeave);

    if(mode == UserStatusBarWidget::Mode::leaveAndOk) {
        this->connect(this->userStatusBar, &UserStatusBarWidget::ok, this, &PageWithUserBar::onOk);
    }
}

void PageWithUserBar::setupUserBar(QLayout *layout) {
    layout->addWidget(this->userStatusBar);
}

void PageWithUserBar::onExtraButton(int id) { Q_UNUSED(id) }

void PageWithUserBar::onLeave() {
    if(this->userStatusBar->mode() == UserStatusBarWidget::Mode::logout) {
        qDebug() << "logout called from" << this->sender();
        this->stateManager->logout();
    }
}

void PageWithUserBar::onOk() { /* Not used in base class */ }

PageWithUserBar::~PageWithUserBar() {

}
