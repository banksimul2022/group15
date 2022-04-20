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

bool PageInsertCard::processResult(QWidget *page, QVariant result) {
    Q_UNUSED(result);

    if(qobject_cast<PageMainAccountView*>(page) == nullptr) { // Check that the page was PageMainAccountView
        return false;
    }

    this->processReads = true;

    return false;
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
        this->stateManager->displayPrompt(this, "Virhe!", QString::number(data->error()).toUtf8().data(), PromptEnum::error, 0);
        return;
    }

    this->stateManager->navigateToPage(new PageMainAccountView(this->stateManager));
}
