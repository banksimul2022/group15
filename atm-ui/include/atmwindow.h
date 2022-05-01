#ifndef ATMWINDOW_H
#define ATMWINDOW_H

#include "page/pageloading.h"
#include "pagemanager.h"

#include <QMainWindow>
#include <QVariant>
#include <QStack>

QT_BEGIN_NAMESPACE
namespace Ui { class ATMWindow; }
QT_END_NAMESPACE

class ATMWindow : public QMainWindow, public PageManager {
    Q_OBJECT

    public:
        ATMWindow(QWidget *parent = nullptr);
        ~ATMWindow();

        AsyncSerialInterface *getRFIDInterface() override;
        RESTInterface *getRESTInterface(bool displayLoadingPage = true) override;

        void displayPrompt(QString title, QString message, PromptEnum::Icon icon, int btnCount, ...) override;
        PageBase *createPrompt(QString title, QString message, PromptEnum::Icon icon, int btnCount, ...) override;
        PageBase *createPrompt(QString title, QString message, PromptEnum::Icon icon, int btnCount, va_list args) override;

        void leaveLoadingPage() override;

        QVariant navigateToPage(PageBase *page) override;
        bool leaveCurrentPage(QVariant result) override;
        void leaveAllPages(QVariant result) override;

    private:
        // Returns true to end processing
        bool processPageReturnAction(PageReturnAction action, PageBase **newPage);

        void popTopPage(PageBase **oldPage, PageBase **actualPage); // Pops the actual top page (Check for loading page at top of stack)
        void setPage(PageBase *page, PageBase *oldPage = nullptr);
        void deletePage(PageBase *page, PageBase *page2 = nullptr); // Protects the loading page from getting deleted
        void displayLoadingPage();

        QSettings settings;

        AsyncSerialInterface *rfidInterface;
        RESTInterface *restInterface;

        QStack<PageBase*> pageStack;
        PageLoading *loadingPage;

        Ui::ATMWindow *ui;
        QString baseTitle;

    signals:
        void onRestData(RestReturnData **data);

    public slots:
        void showCursorShortcut();
        void fullscreenShortcut();

    private slots:
        void onRestDataFromDLL(RestReturnData *data);
};
#endif // ATMWINDOW_H
