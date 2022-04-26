#include "page/abstract/pagebase.h"

PageBase::PageBase(StateManager *stateManager, QWidget *parent) : QWidget{parent} {
    this->setVisible(false); // Prevent pages from randomly opening in their own windows when deparented
    this->stateManager = stateManager;
    this->connect(this->stateManager->getRESTInterface(false), &RESTInterface::dataReturn, this, &PageBase::onRestData);
}

QVariant PageBase::onNaviagte(const QMetaObject *oldPage, bool closed, QVariant *result) {
    Q_UNUSED(oldPage) Q_UNUSED(closed) Q_UNUSED(result)
    return QVariant::fromValue(StateManager::Stay);
}

void PageBase::onShown() {
    if(!this->hasBeenShown) {
        this->hasBeenShown = true;
        this->onReady();
    }
}

void PageBase::onRestData(RestReturnData *data) { Q_UNUSED(data) }

void PageBase::onReady() { /* Not used in base class */ }

PageBase::~PageBase() { }
