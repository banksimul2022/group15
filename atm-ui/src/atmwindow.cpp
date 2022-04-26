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

QWidget *ATMWindow::createPrompt(QString title, QString message, PromptEnum::Icon icon, int btnCount, ...) {
    va_list args;
    va_start(args, btnCount);
    return this->createPrompt(title, message, icon, btnCount, args);
    va_end(args);
}

QWidget *ATMWindow::createPrompt(QString title, QString message, PromptEnum::Icon icon, int btnCount, va_list args) {
    return new PagePrompt(title, message, icon, btnCount, args, this);
}

void ATMWindow::displayPrompt(QString title, QString message, PromptEnum::Icon icon, int btnCount, ...) {
    va_list args;
    va_start(args, btnCount);
    this->navigateToPage(this->createPrompt(title, message, icon, btnCount, args));
    va_end(args);
}

void ATMWindow::leaveLoadingPage() {
    if(this->pageStack.top() != this->loadingPage) {
        return;
    }

    QWidget* loadingPage = this->pageStack.pop();
    this->setPage(this->pageStack.top(), loadingPage);
}

QVariant ATMWindow::navigateToPage(QWidget *page) {
    Q_ASSERT(page != nullptr);
    QWidget *currentPage = this->pageStack.isEmpty() ? nullptr : this->pageStack.top();
    PageBase *pageCast = qobject_cast<PageBase*>(page);

    qDebug() << "----BEF--------";
    foreach(QWidget *p, this->pageStack) {
        qDebug() << p;
    }
    qDebug() << "----------------";

    QVariant result;
    result = pageCast == nullptr ? false : pageCast->onNaviagte(page->metaObject(), false, &result);
    Q_ASSERT_X(result.userType() == qMetaTypeId<StateManager::PageReturnAction>(), "ATMWindow::leaveCurrentPage", "Unknown return type from page");

    QWidget *newPage = page;

    if(this->pageStack.length() > 0 && this->pageStack.top() == this->loadingPage) {
        this->pageStack.pop();
    }

    PageReturnAction action = result.value<StateManager::PageReturnAction>();

    if(!this->processPageReturnAction(action, &newPage)) {
        return result;
    }

    if(action != StateManager::KeepLoading) {
        this->pageStack.push(page);
    }

    this->setPage(newPage, currentPage);

    qDebug() << "---AFT----------";
    foreach(QWidget *p, this->pageStack) {
        qDebug() << p;
    }
    qDebug() << "----------------";

    if(pageCast != nullptr) {
        pageCast->onShown();
    }

    return QVariant();
}

bool ATMWindow::leaveCurrentPage(QVariant result) {
    if(this->pageStack.length() < 2) {
        return false; // There must be atleast one page on the stack (PageInsertCard)
    }

    qDebug() << "--BEF-----------";
    foreach(QWidget *p, this->pageStack) {
        qDebug() << p;
    }
    qDebug() << "----------------";

    bool pushToTop = true;
    PageBase *base;
    const QMetaObject *returningPage;
    QWidget *oldPage, *actualPage, *newPage;
    this->popTopPage(&oldPage, &actualPage);
    returningPage = actualPage->metaObject();
    newPage = actualPage;

    do {
        if(result.userType() == qMetaTypeId<StateManager::PageReturnAction>()) {
            PageReturnAction action = result.value<StateManager::PageReturnAction>();
            if(newPage != actualPage && this->processPageReturnAction(action, &newPage)) {
                pushToTop = action != PageReturnAction::KeepLoading;
                break;
            }
        } else if(result.canConvert<QWidget*>()) {
            this->pageStack.push(result.value<QWidget*>());
        }

        if(newPage != actualPage) {
            newPage->deleteLater();
        }

        newPage = this->pageStack.pop();
        base = qobject_cast<PageBase*>(newPage);

        if(base == nullptr) {
            break;
        }

        result = base->onNaviagte(returningPage, true, &result);
        returningPage = newPage->metaObject();
    } while(1);

    if(pushToTop) {
        this->pageStack.push(newPage);
    }

    this->setPage(newPage, oldPage);
    this->deletePage(actualPage, oldPage);

    if(base != nullptr) {
        base->onShown();
    }

    qDebug() << "--AFT-----------";
    foreach(QWidget *p, this->pageStack) {
        qDebug() << p;
    }
    qDebug() << "----------------";

    return true;
}

void ATMWindow::leaveAllPages(QVariant result) {
    Q_UNUSED(result);
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
        newPageCast->onNaviagte(actualPage->metaObject(), true, &result);
    }

    this->setPage(newPage, currentPage);
    this->deletePage(currentPage, actualPage);

    if(newPageCast != nullptr) {
        newPageCast->onShown();
    }
}

void ATMWindow::fullscreenShortcut() {
    if(this->isFullScreen()) {
        this->showNormal();
    } else {
        this->showFullScreen();
    }
}

bool ATMWindow::processPageReturnAction(PageReturnAction action, QWidget **newPage) {
    switch(action) {
        case StateManager::Leave:
            return false;
        case StateManager::KeepLoading: {
            bool insert = *newPage != this->pageStack.top();

            if(this->pageStack.top() != this->loadingPage) {
                this->pageStack.push(this->loadingPage);
            }

            if(newPage != nullptr) {
                if(insert) {
                    this->pageStack.insert(this->pageStack.length() - 1, *newPage);
                }

                *newPage = this->loadingPage;
            }

            [[fallthrough]];
        }
        default:
        case StateManager::Stay:
            return true;
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
    if(!page || page == oldPage) {
        return;
    }

    if(oldPage) {
        PageWithUserBar *pageCast = qobject_cast<PageWithUserBar*>(oldPage);
        if(pageCast != nullptr) pageCast->stopTimer();
        oldPage->setVisible(false); // Prevent page from opening as seperate window
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
    page->setVisible(true);

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
    this->pageStack.push(this->loadingPage);
}
