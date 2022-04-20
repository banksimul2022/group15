#include "page/abstract/pagebase.h"

PageBase::PageBase(StateManager *stateManager, QWidget *parent) : QWidget{parent} {
    this->stateManager = stateManager;
}

bool PageBase::processResult(QWidget *page, QVariant result) {
    Q_UNUSED(page) Q_UNUSED(result)
    return false;
}

PageBase::~PageBase() {

}
