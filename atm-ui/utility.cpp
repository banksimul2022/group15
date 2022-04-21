#include "utility.h"

#include <QVariant>
#include <QStyle>

void Utility::retainSizeWhenHidden(QWidget *widget) {
    QSizePolicy sizePolicy = widget->sizePolicy();
    sizePolicy.setRetainSizeWhenHidden(true);
    widget->setSizePolicy(sizePolicy);
}

void Utility::updateVisualProperty(const char *name, QVariant value, QWidget *widget) {
    widget->setProperty(name, value);
    QStyle *style = widget->style();
    style->unpolish(widget);
    style->polish(widget);
}
