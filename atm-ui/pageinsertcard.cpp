#include "pageinsertcard.h"
#include "ui_pageinsertcard.h"

PageInsertCard::PageInsertCard(StateManager *stateManager, QWidget *parent) : PageBase(stateManager, parent), ui(new Ui::PageInsertCard) {
    ui->setupUi(this);
}

PageInsertCard::~PageInsertCard() {
    delete ui;
}
