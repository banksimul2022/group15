#include "page/abstract/pagebase.h"
#include "utility.h"
#include <QMetaMethod>
#include <QDebug>
#include "page/util/pagereturn.h"

PageBase::PageBase(PageManager *pageManager, QWidget *parent) :
    QWidget{parent},
    hasBeenShown(false)
{
    this->setVisible(false); // Prevent pages from randomly opening in their own windows when deparented
    this->pageManager = pageManager;
    qDebug() << "Create: " << this;
}

QVariant PageBase::onNaviagte(const QMetaObject *oldPage, bool closed, QVariant *result) {
    Q_UNUSED(oldPage) Q_UNUSED(closed) Q_UNUSED(result)
    return QVariant::fromValue(PageManager::Stay);
}

void PageBase::onShown() {
    if(!this->hasBeenShown) {
        this->hasBeenShown = true;
        this->onReady();
    }
}

PageBase::RestDataAction PageBase::onRestData(RestReturnData *data) { Q_UNUSED(data) return RestDataAction::Skip; }

void PageBase::retranslate() { }

bool PageBase::handleRestError(RestReturnData *data, QString action, bool leave) {
    if(data->error() == -1) return false;

    PageBase *prompt = this->pageManager->createPrompt(
                          tr("Verkkovirhe"),
                          tr("Odottamaton virhe %1!\n%2! (%3)").arg(action, Utility::restErrorToText(data->error()), QString::number(data->error())),
                          PromptEnum::error, 0
                      );

    if(leave) {
        this->pageManager->leaveCurrentPage(QVariant::fromValue(new PageReturn(prompt, PageReturn::LeaveCurrent)));
    } else {
        this->pageManager->navigateToPage(prompt);
    }

    return true;
}

void PageBase::onReady() { /* Not used in base class */ }

PageBase::~PageBase() { qDebug() << "Destroy: " << this; }
