#ifndef PAGEWIDGETWRAPPER_H
#define PAGEWIDGETWRAPPER_H

#include "page/abstract/pagebase.h"

#include <QObject>
#include <QVBoxLayout>

class PageWidgetWrapper : public PageBase {
    Q_OBJECT

    public:
        explicit PageWidgetWrapper(QWidget *widget, PageManager *pageManager, QWidget *parent = nullptr);

        QWidget *widget();

    private:
        QVBoxLayout *rootLayout;
        QWidget *heldWidget;
};

#endif // PAGEWIDGETWRAPPER_H
