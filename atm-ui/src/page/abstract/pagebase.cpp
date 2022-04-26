#include "page/abstract/pagebase.h"
#include "utility.h"

PageBase::PageBase(StateManager *stateManager, QWidget *parent) :
    QWidget{parent},
    hasBeenShown(false)
{
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

bool PageBase::handleRestError(RestReturnData *data, QString action, bool leave) {
    if(data->error() == -1) return false;

    QWidget *prompt = this->stateManager->createPrompt(
                          tr("Verkkovirhe"),
                          tr("Odottamaton virhe %1!\n%2! (%3)").arg(action, Utility::restErrorToText(data->error()), QString::number(data->error())),
                          PromptEnum::error, 0
                      );

    if(leave) {
        this->stateManager->leaveCurrentPage(QVariant::fromValue(prompt));
    } else {
        this->stateManager->navigateToPage(prompt);
    }

    delete data;

    return true;
}

void PageBase::onReady() { /* Not used in base class */ }

PageBase::~PageBase() { }
