#include "page/util/pagereturn.h"

PageReturn::PageReturn(PageBase *page, Action action) : pageWidget(page), navigateAction(action) {}

PageBase *PageReturn::page() {
    return this->pageWidget;
}

PageReturn::Action PageReturn::action() {
    return this->navigateAction;
}
