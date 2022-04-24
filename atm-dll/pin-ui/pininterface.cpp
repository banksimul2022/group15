#include "pininterface.h"
#include "pinwidget.h"

PinInterface *PinInterface::createWidgetInstance(QWidget *parent)
{
    return new PinWidget(parent);
}

PinInterface::PinInterface(QWidget *parent) : QWidget{parent} {}

PinInterface::~PinInterface() {}

