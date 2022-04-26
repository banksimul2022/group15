#include "page/pageinsertcard.h"
#include "ui_pageinsertcard.h"

#include <QDebug>
#include <resterrorcode.h>

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
    Q_UNUSED(oldPage) Q_UNUSED(closed) Q_UNUSED(result)

    this->processReads = true;

    return QVariant::fromValue(StateManager::Stay);
}

void PageInsertCard::onCardRead(QString number) {
    if(!this->processReads) {
        return; // Should we use disconnect instead of using a bool flag????
    }

    this->processReads = false;

    // TODO: Replace with pinui page
    this->stateManager->getRESTInterface()->login(number, "95959");
}

void PageInsertCard::onRestData(RestReturnData *data) {
    if(data->type() != RestReturnData::typeLogin) return;

    if(data->error() == RestErrors::ERR_CARD_LOCKED) {
        this->stateManager->displayPrompt(
            tr("Kortti lukittu"),
            tr("Korttisi on lukittu liian monen virheellisen kirjautumisen takia!\nOta yhteyttä henkilökuntaan"),
            PromptEnum::warning, 0
        );
    } else if(data->error() == RestErrors::ERR_INVALID_CREDENTIALS) {
        this->stateManager->displayPrompt(
            tr("Virheelliset tunnukset"),
            tr("PIN koodisi tai kortin numero on virhellinen!\nYritä uudelleen..."),
            PromptEnum::warning, 0
        );
    } else if(this->handleRestError(data, tr("kirjauduttaessa"), false)) {
        return;
    } else {
        this->navigate<PageMainAccountView>();
    }

    delete data;
}
