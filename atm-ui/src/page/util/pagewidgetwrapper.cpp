#include "page/util/pagewidgetwrapper.h"

PageWidgetWrapper::PageWidgetWrapper(QWidget *widget, PageManager *pageManager, QWidget *parent) :
      PageBase{pageManager, parent},
      rootLayout(new QVBoxLayout(this)),
      heldWidget(widget)
{
    this->rootLayout->setObjectName("layout");
    this->heldWidget->setParent(this); // No need to delete in destructor as the widget is set as a child of this page
    this->rootLayout->addWidget(this->heldWidget);
}

QWidget *PageWidgetWrapper::widget() {
    return this->heldWidget;
}
