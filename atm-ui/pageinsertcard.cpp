#include "pageinsertcard.h"
#include "ui_pageinsertcard.h"

PageInsertCard::PageInsertCard(QWidget *parent) : QWidget(parent), ui(new Ui::PageInsertCard) {
    ui->setupUi(this);
}

PageInsertCard::~PageInsertCard() {
    delete ui;
}
