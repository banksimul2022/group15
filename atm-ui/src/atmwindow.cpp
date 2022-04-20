#include "atmwindow.h"
#include "ui_atmwindow.h"
#include "page/pageinsertcard.h"

#include <QShortcut>

#include "page/pageprompt.h"

ATMWindow::ATMWindow(QWidget *parent) :
    QMainWindow(parent),
    rfidInterface(new RFIDInterface),
    restInterface(RESTInterface::createInstance()),
    loadingPage(new PageLoading(this)),
    ui(new Ui::ATMWindow)
{
    ui->setupUi(this);
    this->loadingPage->setVisible(false); // To prevent loading page from randomly opening in it's own window
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

RESTInterface *ATMWindow::getRESTInterface(bool displayLoadingPage) {
    if(displayLoadingPage) {
        this->setPage(this->loadingPage, this->pageStack.top());
        this->loadingPage->setVisible(true);
        this->pageStack.push(this->loadingPage);
    }

    return this->restInterface;
}

void ATMWindow::displayPrompt(QObject *ctx, const char *title, const char *message, PromptEnum::Icon icon, int btnCount, ...) {
    va_list args;
    va_start(args, btnCount);
    this->navigateToPage(new PagePrompt(ctx->metaObject()->className(), title, message, icon, btnCount, args, this));
    va_end(args);
}

void ATMWindow::navigateToPage(QWidget *page) {
    Q_ASSERT(page != nullptr);
    QWidget *currentPage = this->pageStack.isEmpty() ? nullptr : this->pageStack.top();

    if(currentPage == this->loadingPage) {
        this->pageStack.pop();
        currentPage->setVisible(false);
    }

    this->pageStack.push(page);
    this->setPage(page, currentPage);
}

bool ATMWindow::leaveCurrentPage(QVariant result) {
    if(this->pageStack.length() < 2) {
        return false; // There must be atleast one page on the stack (PageInsertCard)
    }

    QWidget *oldPage = this->pageStack.pop();
    QWidget *actualPage = oldPage;

    if(oldPage == this->loadingPage) {
        Q_ASSERT(this->pageStack.length() > 1);
        actualPage = this->pageStack.pop();
        oldPage->setVisible(false);
    }

    QWidget *newPage = this->pageStack.top();

    PageBase *page;
    while((page = qobject_cast<PageBase*>(newPage)) != nullptr && page->processResult(actualPage, result) && this->pageStack.length() > 1) {
        this->pageStack.pop();
        newPage->deleteLater();
        newPage = this->pageStack.top();
    }

    this->setPage(newPage, oldPage);
    if(actualPage != oldPage) actualPage->deleteLater();
    if(oldPage != this->loadingPage) oldPage->deleteLater(); // Use delete later in case the method caller is oldPage

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
