#include "page/abstract/pagebase.h"

PageBase::PageBase(StateManager *stateManager, QWidget *parent) : QWidget{parent} {
    this->stateManager = stateManager;
    this->connect(this->stateManager->getRESTInterface(false), &RESTInterface::dataReturn, this, &PageBase::onRestData);
}

bool PageBase::processResult(QWidget *page, QVariant result) {
    Q_UNUSED(page) Q_UNUSED(result)
    return false;
}

void PageBase::onRestData(RestReturnData *data) { Q_UNUSED(data) }

PageBase::~PageBase() { }
