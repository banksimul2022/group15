#include "atmwindow.h"
#include "ui_atmwindow.h"
#include "page/pageinsertcard.h"

#include <QShortcut>
#include <QDebug>

ATMWindow::ATMWindow(QWidget *parent) :
    QMainWindow(parent),
    rfidInterface(new RFIDInterface),
    restInterface(RESTInterface::createInstance()),
    ui(new Ui::ATMWindow)
{
    ui->setupUi(this);
    new QShortcut(QKeySequence(Qt::Key_F11), this, SLOT(fullscreenShortcut()));
    this->baseTitle = this->windowTitle();
    this->navigateToPage(new PageInsertCard(this));
}

ATMWindow::~ATMWindow() {
    delete ui;
}

RFIDInterface *ATMWindow::getRFIDInterface() {
    return this->rfidInterface;
}

RESTInterface *ATMWindow::getRESTInterface() {
    return this->restInterface;
}

void ATMWindow::navigateToPage(QWidget *page) {
    Q_ASSERT(page != nullptr);
    QWidget *currentPage = this->pageStack.top();
    this->pageStack.push(page);
    this->setPage(page, currentPage);
}

bool ATMWindow::leaveCurrentPage(QVariant result) {
    if(this->pageStack.length() < 2) {
        return false; // There must be atleast one page on the stack (PageInsertCard)
    }

    QWidget *oldPage = this->pageStack.pop();
    this->setPage(this->pageStack.top(), oldPage);
    oldPage->deleteLater(); // Use delete later in case the method caller is oldPage

    return true;
}

void ATMWindow::fullscreenShortcut() {
    if(this->isFullScreen()) {
        this->showNormal();
    } else {
        this->showFullScreen();
    }
}

void ATMWindow::setPage(QWidget *page, QWidget *oldPage) {
    if(!page) {
        return;
    }

    if(oldPage) {
        this->ui->rootLayout->removeWidget(oldPage);
        oldPage->setParent(nullptr);
    }

    this->setWindowTitle(QString("%1 - %2").arg(this->baseTitle, page->windowTitle()));

    QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(page->sizePolicy().hasHeightForWidth());
    page->setSizePolicy(sizePolicy);

    page->setParent(this);
    this->ui->rootLayout->addWidget(page);
}
