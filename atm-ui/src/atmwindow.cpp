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
        this->displayLoadingPage();
    }

    return this->restInterface;
}

void ATMWindow::displayPrompt(QObject *ctx, const char *title, const char *message, PromptEnum::Icon icon, int btnCount, ...) {
    va_list args;
    va_start(args, btnCount);
    this->navigateToPage(new PagePrompt(ctx->metaObject()->className(), title, message, icon, btnCount, args, this));
    va_end(args);
}

void ATMWindow::leaveLoadingPage() {
    if(this->pageStack.top() != this->loadingPage) {
        return;
    }

    QWidget* loadingPage = this->pageStack.pop();
    this->setPage(this->pageStack.top(), loadingPage);
}

void ATMWindow::navigateToPage(QWidget *page) {
    Q_ASSERT(page != nullptr);
    QWidget *currentPage = this->pageStack.isEmpty() ? nullptr : this->pageStack.top();
    PageBase *pageCast = qobject_cast<PageBase*>(page);
    bool keep = pageCast == nullptr ? false : pageCast->keepLoadingPageOnNavigate();

    if(currentPage == this->loadingPage) {
        if(keep) {
            pageStack.insert(pageStack.length() - 1, page); // Add page before loading page
            return;
        } else {
            this->pageStack.pop();
            currentPage->setVisible(false);
        }
    } else if(keep) {
        this->displayLoadingPage();
        pageStack.insert(pageStack.length() - 1, page); // Add page before loading page
        return;
    }

    this->pageStack.push(page);
    this->setPage(page, currentPage);
    if(pageCast != nullptr) pageCast->onNavigate();
}

bool ATMWindow::leaveCurrentPage(QVariant result) {
    if(this->pageStack.length() < 2) {
        return false; // There must be atleast one page on the stack (PageInsertCard)
    }

    QWidget *oldPage = nullptr;
    QWidget *actualPage = nullptr;

    this->popTopPage(&oldPage, &actualPage);

    QWidget *newPage = this->pageStack.top();

    PageBase *page;
    while((page = qobject_cast<PageBase*>(newPage)) != nullptr && page->processResult(actualPage, result) && this->pageStack.length() > 1) {
        this->pageStack.pop();
        this->deletePage(newPage);
        newPage = this->pageStack.top();
    }

    this->setPage(newPage, oldPage);
    this->deletePage(oldPage, actualPage);

    return true;
}

void ATMWindow::leaveAllPages(QVariant result) {
    Q_ASSERT(this->pageStack.length() > 1);
    QWidget *currentPage = nullptr;
    QWidget *actualPage = nullptr;

    this->popTopPage(&currentPage, &actualPage);

    while(this->pageStack.length() > 1) {
        this->deletePage(this->pageStack.pop());
    }

    QWidget *newPage = this->pageStack.top();
    PageBase *newPageCast = qobject_cast<PageBase*>(newPage);

    if(newPageCast != nullptr) {
        newPageCast->processResult(actualPage, result);
    }

    this->setPage(newPage, currentPage);
    this->deletePage(currentPage, actualPage);
}

void ATMWindow::fullscreenShortcut() {
    if(this->isFullScreen()) {
        this->showNormal();
    } else {
        this->showFullScreen();
    }
}

void ATMWindow::popTopPage(QWidget **oldPage, QWidget **actualPage) {
    *oldPage = this->pageStack.pop();
    *actualPage = *oldPage;

    if(*oldPage == this->loadingPage) {
        Q_ASSERT(this->pageStack.length() > 1);
        *actualPage = this->pageStack.pop();
        (*oldPage)->setVisible(false);
    }
}

void ATMWindow::setPage(QWidget *page, QWidget *oldPage) {
    if(!page) {
        return;
    }

    if(oldPage) {
        PageWithUserBar *pageCast = qobject_cast<PageWithUserBar*>(oldPage);
        if(pageCast != nullptr) pageCast->stopTimer();
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

    PageWithUserBar *pageCast = qobject_cast<PageWithUserBar*>(page);
    if(pageCast != nullptr) pageCast->startTimer();
}

void ATMWindow::deletePage(QWidget *page, QWidget *page2) {
    if(page2 != nullptr && page2 != this->loadingPage && page2 != page) page2->deleteLater();
    if(page != this->loadingPage) page->deleteLater();
}

void ATMWindow::displayLoadingPage() {
    if(this->pageStack.top() == this->loadingPage) {
        return;
    }

    this->setPage(this->loadingPage, this->pageStack.top());
    this->loadingPage->setVisible(true);
    this->pageStack.push(this->loadingPage);
}
