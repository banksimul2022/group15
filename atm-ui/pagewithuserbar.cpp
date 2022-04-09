#include "pagewithuserbar.h"

PageWithUserBar::PageWithUserBar(StateManager *stateManager, QWidget *parent) : PageBase{stateManager, parent} {
    this->userStatusBar = new UserStatusBarWidget(this);
    this->connect(this->userStatusBar, &UserStatusBarWidget::logout, this, &PageWithUserBar::logout);
}

void PageWithUserBar::setupUserBar(QLayout *layout) {
    layout->addWidget(this->userStatusBar);
}

void PageWithUserBar::logout() {
    this->stateManager->logout();
}

PageWithUserBar::~PageWithUserBar() {

}
