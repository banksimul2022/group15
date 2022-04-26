#include "page/pageinsertcard.h"
#include "ui_pageinsertcard.h"
#include "page/pageloading.h"
#include "page/pagemainaccountview.h"

#include <resterrorcode.h>
#include <pininterface.h>

PageInsertCard::PageInsertCard(StateManager *stateManager, QWidget *parent) :
    PageBase(stateManager, parent),
    processReads(true),
    ui(new Ui::PageInsertCard)
{
    ui->setupUi(this);
    this->number = nullptr;
    this->pin = nullptr;
    this->connect(this->stateManager->getRFIDInterface(), &RFIDInterface::cardRead, this, &PageInsertCard::onCardRead);
}

PageInsertCard::~PageInsertCard() {
    delete ui;
}

QVariant PageInsertCard::onNaviagte(const QMetaObject *oldPage, bool closed, QVariant *result) {
    Q_UNUSED(oldPage) Q_UNUSED(closed) Q_UNUSED(result)

    if(this->number != nullptr && this->pin != nullptr) {
        this->stateManager->getRESTInterface(false)->login(this->number, this->pin);
        this->number = nullptr;
        this->pin = nullptr;
        return QVariant::fromValue(StateManager::KeepLoading);
    } else {
        this->processReads = true;
    }

    return QVariant::fromValue(StateManager::Stay);
}

void PageInsertCard::onCardRead(QString number) {
    if(!this->processReads) {
        return; // Should we use disconnect instead of using a bool flag????
    }

    this->processReads = false;
    this->number = number;

    // We don't need to hold a reference to the pin widget instance as it is automatocally dealt with by the StateManager
    PinInterface *pinWidget = PinInterface::createWidgetInstance(this);
    this->connect(pinWidget, &PinInterface::pinWidgetUserInput, this, &PageInsertCard::onPinRead);
    this->connect(pinWidget, &PinInterface::deletePinWidget, this, &PageInsertCard::onPinCancel);
    this->stateManager->navigateToPage(pinWidget);
}

void PageInsertCard::onPinRead(QString pin) {
    this->pin = pin;
    this->stateManager->leaveCurrentPage(QVariant::fromValue(StateManager::Leave));
}

void PageInsertCard::onPinCancel() {
    this->number = nullptr;
    this->stateManager->leaveCurrentPage(QVariant::fromValue(StateManager::Leave));
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
