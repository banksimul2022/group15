#include "utility.h"

void Utility::retainSizeWhenHidden(QWidget *widget) {
    QSizePolicy sizePolicy = widget->sizePolicy();
    sizePolicy.setRetainSizeWhenHidden(true);
    widget->setSizePolicy(sizePolicy);
}
