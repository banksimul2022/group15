#include "page/abstract/pagebase.h"

PageBase::PageBase(StateManager *stateManager, QWidget *parent) : QWidget{parent} {
    this->stateManager = stateManager;
    this->connect(this->stateManager->getRESTInterface(false), &RESTInterface::dataReturn, this, &PageBase::onRestData);
}

void PageBase::onNavigate() { /* Unused in base class */ }

bool PageBase::processResult(QWidget *page, QVariant result) {
    Q_UNUSED(page) Q_UNUSED(result)
    return false;
}

bool PageBase::keepLoadingPageOnNavigate() { return false; }

void PageBase::onRestData(RestReturnData *data) { Q_UNUSED(data) }

PageBase::~PageBase() { }
