#include "atmwindow.h"
#include "ui_atmwindow.h"
#include "page/pageinsertcard.h"
#include "page/pageprompt.h"
#include "pagereturn.h"

#include <QMetaMethod>
#include <QShortcut>
#include <QThread>

ATMWindow::ATMWindow(QWidget *parent) :
    QMainWindow(parent),
    rfidInterface(new AsyncSerialInterface),
    restInterface(RESTInterface::createInstance()),
    loadingPage(new PageLoading(this)),
    ui(new Ui::ATMWindow)
{
    ui->setupUi(this);
    new QShortcut(QKeySequence(Qt::Key_F11), this, SLOT(fullscreenShortcut()));
    this->baseTitle = this->windowTitle();
    this->navigateToPage(new PageInsertCard(this));
    this->connect(this->restInterface, &RESTInterface::dataReturn, this, &ATMWindow::onRestDataFromDLL);
}

ATMWindow::~ATMWindow() {
    delete ui;
}

AsyncSerialInterface *ATMWindow::getRFIDInterface() {
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

    QVariant result;
    result = pageCast == nullptr ? false : pageCast->onNaviagte(page->metaObject(), false, &result);
    Q_ASSERT_X(
        pageCast == nullptr || result.userType() == qMetaTypeId<PageManager::PageReturnAction>(),
        "ATMWindow::leaveCurrentPage", "Unknown return type from page"
    );

    QWidget *newPage = page;

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

bool ATMWindow::leaveCurrentPage(QVariant result) {
    if(this->pageStack.length() < 2) {
        return false; // There must be atleast one page on the stack (PageInsertCard)
    }

    bool pushToTop = true;
    PageBase *base;
    const QMetaObject *returningPage;
    QWidget *oldPage, *actualPage, *newPage;
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

    if(this->pageStack.length() < 2 || this->pageStack.at(this->pageStack.length() - 2) != actualPage) {
        this->deletePage(actualPage, oldPage);
    }

    if(base != nullptr) {
        base->onShown();
    }

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

    QWidget *newPage;

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

void ATMWindow::fullscreenShortcut() {
    if(this->isFullScreen()) {
        this->showNormal();
    } else {
        this->showFullScreen();
    }
}

void ATMWindow::onRestDataFromDLL(RestReturnData *data) {
    if(data->type() == RestReturnData::typeinternalerror) {
        this->leaveAllPages(
            QVariant::fromValue(new PageReturn(
                this->createPrompt(
                    tr("Sisäinen virhe!"),
                    tr("Odottamaton verkko virhe! (%1)").arg(data->error()),
                    PromptEnum::error, 0
                ), PageReturn::LeaveCurrent
            ))
        );
        return;
    }

    int i;

    for(i = this->pageStack.size() - 1; i >= 0; i--) {
        PageBase *page = qobject_cast<PageBase*>(this->pageStack.at(i));

        if(page == nullptr) {
            continue;
        }

        switch(page->onRestData(data)) {
            case PageBase::RestDataAction::Skip:
                break;
            case PageBase::RestDataAction::Delete:
                qDebug() << "Delete in page";
                delete data;
                Q_FALLTHROUGH();
            case PageBase::RestDataAction::SetNull: // Really just means don't delete put stop propagation
                qDebug() << "Set to null in page";
                i = -2;
                break;
        }
    }

    if(i > -2) { // Means that the data was not deleted in the loop
        delete data;
    }
}

bool ATMWindow::processPageReturnAction(PageReturnAction action, QWidget **newPage) {
    switch(action) {
        case PageManager::Leave:
            return false;
        case PageManager::KeepLoading: {
            bool insert = this->pageStack.isEmpty() || *newPage != this->pageStack.top();

            if(this->pageStack.isEmpty() || this->pageStack.top() != this->loadingPage) {
                this->pageStack.push(this->loadingPage);
            }

            if(newPage != nullptr) {
                if(insert) {
                    this->pageStack.insert(this->pageStack.length() - 1, *newPage);
                }

                *newPage = this->loadingPage;
            }

            Q_FALLTHROUGH();
        }
        default:
        case PageManager::Stay:
            return true;
    }
}

void ATMWindow::popTopPage(QWidget **oldPage, QWidget **actualPage) {
    *oldPage = this->pageStack.pop();
    *actualPage = *oldPage;

    if(*oldPage == this->loadingPage) {
        *actualPage = this->pageStack.length() > 1 ? this->pageStack.pop() : nullptr;
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
