#include "page/pageinsertcard.h"
#include "ui_pageinsertcard.h"
#include "page/pageloading.h"
#include "page/pagemainaccountview.h"
#include "page/util/pagewidgetwrapper.h"
#include "utility.h"

#include <resterrorcode.h>
#include <pininterface.h>
#include <QSvgRenderer>

PageInsertCard::PageInsertCard(PageManager *pageManager, QWidget *parent) :
    PageBase(pageManager, parent),
    processReads(true),
    ui(new Ui::PageInsertCard)
{
    ui->setupUi(this);

    this->ui->svgCard->load(QString(":files/icons/card.svg"));
    this->ui->svgCard->renderer()->setAspectRatioMode(Qt::KeepAspectRatio);

    this->number = nullptr;
    this->pin = nullptr;

    this->connect(this->pageManager->getRFIDInterface(), &AsyncSerialInterface::CardID, this, &PageInsertCard::onCardRead);
}

PageInsertCard::~PageInsertCard() {
    delete ui;
}

void PageInsertCard::onShown() {
    PageBase::onShown();
    Utility::switchLanguage("fi");
    this->pageManager->retranslate();
}

QVariant PageInsertCard::onNaviagte(const QMetaObject *oldPage, bool closed, QVariant *result) {
    Q_UNUSED(oldPage) Q_UNUSED(closed) Q_UNUSED(result)

    if(this->number != nullptr && this->pin != nullptr) {
        this->pageManager->getRESTInterface(false)->login(this->number, this->pin);
        this->number = nullptr;
        this->pin = nullptr;
        return QVariant::fromValue(PageManager::KeepLoading);
    } else {
        this->processReads = true;
    }

    return QVariant::fromValue(PageManager::Stay);
}

void PageInsertCard::retranslate() {
    this->ui->retranslateUi(this);
}

void PageInsertCard::onCardRead(QString number) {
    if(!this->processReads) {
        return; // Should we use disconnect instead of using a bool flag????
    }

    this->processReads = false;
    this->number = number;

    // We don't need to hold a reference to the pin widget instance as it is automatocally dealt with by the StateManager
    PinInterface *pinWidget = PinInterface::createWidgetInstance();
    this->connect(pinWidget, &PinInterface::pinWidgetUserInput, this, &PageInsertCard::onPinRead);
    this->connect(pinWidget, &PinInterface::deletePinWidget, this, &PageInsertCard::onPinCancel);
    this->navigate<PageWidgetWrapper>(pinWidget);
}

void PageInsertCard::onPinRead(QString pin) {
    this->pin = pin;
    this->pageManager->leaveCurrentPage(QVariant::fromValue(PageManager::Leave));
}

void PageInsertCard::onPinCancel() {
    this->number = nullptr;
    this->pageManager->leaveCurrentPage(QVariant::fromValue(PageManager::Leave));
}

PageBase::RestDataAction PageInsertCard::onRestData(RestReturnData *data) {
    if(data->type() != RestReturnData::typeLogin) return RestDataAction::Skip;

    if(data->error() == RestErrors::ERR_CARD_LOCKED) {
        this->pageManager->displayPrompt(
            tr("Kortti lukittu"),
            tr("Korttisi on lukittu liian monen virheellisen kirjautumisen takia!\nOta yhteyttä henkilökuntaan"),
            PromptEnum::warning, 0
        );
    } else if(data->error() == RestErrors::ERR_INVALID_CREDENTIALS) {
        this->pageManager->displayPrompt(
            tr("Virheelliset tunnukset"),
            tr("PIN koodisi tai kortin numero on virhellinen!\nYritä uudelleen..."),
            PromptEnum::warning, 0
        );
    } else if(this->handleRestError(data, tr("kirjauduttaessa"), false)) {
        return RestDataAction::Skip;
    } else {
        this->navigate<PageMainAccountView>();
    }

    return RestDataAction::Delete;
}

void PageInsertCard::on_btnFi_clicked() {
    Utility::switchLanguage("fi");
    this->pageManager->retranslate();
}


void PageInsertCard::on_btnEn_clicked() {
    Utility::switchLanguage("en-GB");
    this->pageManager->retranslate();
}

