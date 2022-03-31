#include "pinuiwidget.h"
#include "ui_pinuiwidget.h"

PinUIWidget::PinUIWidget(QWidget *parent) : QWidget(parent), ui(new Ui::PinUIWidget) {
    ui->setupUi(this);
}

PinUIWidget::~PinUIWidget() {
    delete ui;
}
