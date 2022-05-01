#include "atmwindow.h"
#include "ui_atmwindow.h"
#include "page/pageinsertcard.h"
#include "page/util/pagereturn.h"
#include "page/abstract/pagewithuserbar.h"

#include <QShortcut>
#include <QDebug>

ATMWindow::ATMWindow(QWidget *parent) :
    QMainWindow(parent),
    settings(QSettings::IniFormat, QSettings::UserScope, "group15", "ATM App"),
    rfidInterface(new AsyncSerialInterface),
    restInterface(RESTInterface::createInstance(&this->settings)),
    loadingPage(new PageLoading(this)),
    ui(new Ui::ATMWindow)
{
    ui->setupUi(this);
    new QShortcut(QKeySequence(Qt::Key_F10), this, SLOT(showCursorShortcut()));
    new QShortcut(QKeySequence(Qt::Key_F11), this, SLOT(fullscreenShortcut()));
    this->baseTitle = this->windowTitle();
    this->navigateToPage(new PageInsertCard(this));
    this->connect(this->restInterface, &RESTInterface::dataReturn, this, &ATMWindow::onRestDataFromDLL);
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

void ATMWindow::setPage(QWidget *page, QWidget *oldPage) {
    if(page != nullptr && page == oldPage) {
        return;
    }

    if(oldPage != nullptr) {
        PageWithUserBar *pageCast = qobject_cast<PageWithUserBar*>(oldPage);
        if(pageCast != nullptr) pageCast->stopTimer();
        oldPage->setVisible(false); // Prevent page from opening as seperate window
        this->ui->rootLayout->removeWidget(oldPage);
        oldPage->setParent(nullptr);
    }

    if(page != nullptr) {
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

void ATMWindow::deletePage(QWidget *page, QWidget *page2) {
    if(page2 != nullptr && page2 != this->loadingPage && page2 != page) page2->deleteLater();
    if(page != this->loadingPage) page->deleteLater();
}

void ATMWindow::showCursorShortcut() {
    QApplication::restoreOverrideCursor();
}

void ATMWindow::fullscreenShortcut() {
    if(this->isFullScreen()) {
        this->showNormal();
        QApplication::restoreOverrideCursor();
    } else {
        this->showFullScreen();
        QApplication::setOverrideCursor(QCursor(Qt::BlankCursor));
    }
}

ATMWindow::~ATMWindow() {
    this->setPage(nullptr, this->pageStack.top());

    while(this->pageStack.length() > 0) {
        QWidget *page = this->pageStack.pop();

        if(page != this->loadingPage) {
            delete page;
        }
    }

    delete this->loadingPage;
    this->loadingPage = nullptr;

    delete this->restInterface;
    this->restInterface = nullptr;
    delete this->rfidInterface;
    this->rfidInterface = nullptr;

    delete ui;
}
