#include "utility.h"

#include <resterrorcode.h>
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

QString Utility::restErrorToText(uint errorCode) {
    switch(errorCode) {
        case RestErrors::ERR_UNKNOWN:
            return QStringLiteral("Tuntematon virhe");
        case RestErrors::ERR_UNKNOWN_PERM_FLAG:
            return QStringLiteral("Tuntematon oikeus");
        case RestErrors::ERR_DATABASE:
            return QStringLiteral("Tietokantavirhe");
        case RestErrors::ERR_INVALID_PARAM:
            return QStringLiteral("Virheellinen pyyntö parametri");
        case RestErrors::ERR_INVALID_CREDENTIALS:
            return QStringLiteral("PIN tai kortin numero on väärin");
        case RestErrors::ERR_INVALID_AUTH:
            return QStringLiteral("Authorisaatio otsaketta ei asetettu");
        case RestErrors::ERR_NOT_ALLOWED:
            return QStringLiteral("Ei oikeutta suorittaa operaatiota");
        case RestErrors::ERR_INSUFFICIENT_FUNDS:
            return QStringLiteral("Tililläsi ei ole katetta");
        case RestErrors::ERR_INVALID_SUM:
            return QStringLiteral("Virheelinen summa");
        case RestErrors::ERR_CREDIT_NOT_SUPPORTED:
            return QStringLiteral("Korttisi ei tue credit ominaisuutta");
        case RestErrors::ERR_CARD_LOCKED:
            return QStringLiteral("Kortti on lukittu");
        case RestErrors::ERR_UNKNOWN_ACCOUNT:
            return QStringLiteral("Tuntematon tili");
    }

    return QStringLiteral("Virhekoodia ei tunnistettu");
}
