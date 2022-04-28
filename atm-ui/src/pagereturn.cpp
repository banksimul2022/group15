#include "pagereturn.h"

PageReturn::PageReturn(QWidget *page, Action action) : pageWidget(page), navigateAction(action) {}

QWidget *PageReturn::page() {
    return this->pageWidget;
}

PageReturn::Action PageReturn::action() {
    return this->navigateAction;
}
