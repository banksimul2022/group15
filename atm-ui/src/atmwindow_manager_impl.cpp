#include "atmwindow.h"
#include "ui_atmwindow.h"
#include "page/dialog/pageprompt.h"
#include "page/util/pagereturn.h"

// ---------- INTERFACE GETTERS START ----------

AsyncSerialInterface *ATMWindow::getRFIDInterface() {
    return this->rfidInterface;
}

RESTInterface *ATMWindow::getRESTInterface(bool displayLoadingPage) {
    if(displayLoadingPage) {
        this->displayLoadingPage();
    }

    return this->restInterface;
}

// ---------- PROMPT HELPERS START ----------

void ATMWindow::displayPrompt(QString title, QString message, PromptEnum::Icon icon, int btnCount, ...) {
    va_list args;
    va_start(args, btnCount);
    this->navigateToPage(this->createPrompt(title, message, icon, btnCount, args));
    va_end(args);
}

PageBase *ATMWindow::createPrompt(QString title, QString message, PromptEnum::Icon icon, int btnCount, ...) {
    va_list args;
    va_start(args, btnCount);
    return this->createPrompt(title, message, icon, btnCount, args);
    va_end(args);
}

PageBase *ATMWindow::createPrompt(QString title, QString message, PromptEnum::Icon icon, int btnCount, va_list args) {
    return new PagePrompt(title, message, icon, btnCount, args, this);
}

// ---------- PAGE STACK MANIPULATION METHODS BEGIN ----------

bool ATMWindow::leaveCurrentPage(QVariant result) {
    if(this->pageStack.length() < 2) {
        return false; // There must be atleast one page on the stack (PageInsertCard)
    }

    bool pushToTop = true;
    const QMetaObject *returningPage;
    PageBase *oldPage, *actualPage, *newPage;
    PageReturn *pReturn = nullptr;
    PageReturn::Action lastAction = PageReturn::LeaveCurrent;

    this->popTopPage(&oldPage, &actualPage);

    returningPage = actualPage->metaObject();
    newPage = actualPage;

    do {
        if(result.userType() == qMetaTypeId<PageManager::PageReturnAction>()) {
            PageReturnAction action = result.value<PageManager::PageReturnAction>();
            if(newPage != actualPage && this->processPageReturnAction(action, &newPage)) {
                pushToTop = action != PageReturnAction::KeepLoading;
                break;
            }
        } else if(result.canConvert<PageReturn*>()) {
            pReturn = result.value<PageReturn*>();

            if(pReturn->action() == PageReturn::AddOnTop) {
                this->pageStack.push(newPage);
            }

            this->pageStack.push(pReturn->page());
        }

        if(newPage != actualPage && (pReturn == nullptr || pReturn->action() != PageReturn::AddOnTop)) {
            newPage->deleteLater();
        }

        newPage = this->pageStack.pop();

        if(lastAction == PageReturn::AddOnTop) {
            this->pageStack.pop(); // Remove older newPage as the page that wanted is leaving
            lastAction = PageReturn::LeaveCurrent;
        }

        if(pReturn != nullptr) {
            lastAction = pReturn->action();
            delete pReturn;
            pReturn = nullptr;
        }

        result = newPage->onNaviagte(returningPage, true, &result);
        returningPage = newPage->metaObject();
    } while(1);

    if(pushToTop) {
        this->pageStack.push(newPage);
    }

    this->setPage(newPage, oldPage);

    if(this->pageStack.length() < 2 || this->pageStack.at(this->pageStack.length() - 2) != actualPage) {
        this->deletePage(actualPage, oldPage);
    }

    newPage->onShown();

    return true;
}

void ATMWindow::leaveAllPages(QVariant result) {
    Q_UNUSED(result);
    Q_ASSERT(this->pageStack.length() > 1);
    PageBase *currentPage = nullptr;
    PageBase *actualPage = nullptr;

    this->popTopPage(&currentPage, &actualPage);

    while(this->pageStack.length() > 1) {
        this->deletePage(this->pageStack.pop());
    }

    PageBase *newPage;

    if(result.canConvert<PageReturn*>()) {
        PageReturn *pReturn = result.value<PageReturn*>();
        newPage = pReturn->page();
        delete pReturn;
        this->pageStack.push(newPage);
    } else {
        newPage = this->pageStack.top();
    }

    PageBase *newPageCast = qobject_cast<PageBase*>(newPage);

    if(newPageCast != nullptr) {
        newPageCast->onNaviagte(actualPage == nullptr ? this->pageStack.at(0)->metaObject() : actualPage->metaObject(), true, &result);
    }

    this->setPage(newPage, currentPage);
    this->deletePage(currentPage, actualPage);

    if(newPageCast != nullptr) {
        newPageCast->onShown();
    }
}

QVariant ATMWindow::navigateToPage(PageBase *page) {
    Q_ASSERT(page != nullptr);
    PageBase *currentPage = this->pageStack.isEmpty() ? nullptr : this->pageStack.top();
    PageBase *pageCast = qobject_cast<PageBase*>(page);

    QVariant result;
    result = pageCast == nullptr ? false : pageCast->onNaviagte(page->metaObject(), false, &result);
    Q_ASSERT_X(
        pageCast == nullptr || result.userType() == qMetaTypeId<PageManager::PageReturnAction>(),
        "ATMWindow::leaveCurrentPage", "Unknown return type from page"
    );

    PageBase *newPage = page;

    if(this->pageStack.length() > 0 && this->pageStack.top() == this->loadingPage) {
        this->pageStack.pop();
    }

    PageReturnAction action = result.value<PageManager::PageReturnAction>();

    if(!this->processPageReturnAction(action, &newPage)) {
        return result;
    }

    if(action != PageManager::KeepLoading) {
        this->pageStack.push(page);
    }

    this->setPage(newPage, currentPage);

    if(pageCast != nullptr) {
        pageCast->onShown();
    }

    return QVariant();
}

void ATMWindow::leaveLoadingPage() {
    if(this->pageStack.top() != this->loadingPage) {
        return;
    }

    PageBase* loadingPage = this->pageStack.pop();
    this->setPage(this->pageStack.top(), loadingPage);
}

void ATMWindow::displayLoadingPage() {
    if(this->pageStack.top() == this->loadingPage) {
        return;
    }

    this->setPage(this->loadingPage, this->pageStack.top());
    this->pageStack.push(this->loadingPage);
}
