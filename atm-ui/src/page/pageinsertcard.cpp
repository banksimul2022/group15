#include "page/pageinsertcard.h"
#include "ui_pageinsertcard.h"

#include <QDebug>

#include "page/pageloading.h"
#include "page/pagemainaccountview.h"

PageInsertCard::PageInsertCard(StateManager *stateManager, QWidget *parent) :
    PageBase(stateManager, parent),
    processReads(true),
    ui(new Ui::PageInsertCard)
{
    ui->setupUi(this);
    this->connect(this->stateManager->getRFIDInterface(), &RFIDInterface::cardRead, this, &PageInsertCard::onCardRead);
}

PageInsertCard::~PageInsertCard() {
    delete ui;
}

QVariant PageInsertCard::onNaviagte(const QMetaObject *oldPage, bool closed, QVariant *result) {
    Q_UNUSED(closed) Q_UNUSED(result)

    if(!closed || !oldPage->inherits(&PageMainAccountView::staticMetaObject)) { // Check that the page was PageMainAccountView
        return QVariant::fromValue(StateManager::Stay);
    }

    this->processReads = true;

    return QVariant::fromValue(StateManager::Stay);
}

void PageInsertCard::onCardRead(QString number) {
    if(!this->processReads) {
        return; // Should we use disconnect instead of using a bool flag????
    }

    this->processReads = false;

    // TODO: Replace with pinui page
    this->stateManager->getRESTInterface()->login(number, "9599");
}

void PageInsertCard::onRestData(RestReturnData *data) {
    if(data->type() != RestReturnData::typeLogin) return;
    int error = data->error();
    delete data;

    if(error != -1) {
        this->stateManager->displayPrompt(tr("Virhe!"), QString::number(data->error()), PromptEnum::error, 0);
        return;
    }

    this->navigate<PageMainAccountView>();
}
