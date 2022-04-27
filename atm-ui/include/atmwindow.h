#ifndef ATMWINDOW_H
#define ATMWINDOW_H

#include "page/pageloading.h"
#include "statemanager.h"

#include <QMainWindow>
#include <QVariant>
#include <QStack>

QT_BEGIN_NAMESPACE
namespace Ui { class ATMWindow; }
QT_END_NAMESPACE

class ATMWindow : public QMainWindow, public StateManager {
    Q_OBJECT

    public:
        ATMWindow(QWidget *parent = nullptr);
        ~ATMWindow();

        RFIDInterface *getRFIDInterface() override;

        RESTInterface *getRESTInterface(bool displayLoadingPage = true) override;
        void connectRestSignal(QObject *receiver, const QMetaMethod slot) override;

        QWidget *createPrompt(QString title, QString message, PromptEnum::Icon icon, int btnCount, ...) override;
        QWidget *createPrompt(QString title, QString message, PromptEnum::Icon icon, int btnCount, va_list args) override;
        void displayPrompt(QString title, QString message, PromptEnum::Icon icon, int btnCount, ...) override;

        void leaveLoadingPage() override;

        QVariant navigateToPage(QWidget *page) override;
        bool leaveCurrentPage(QVariant result) override;
        void leaveAllPages(QVariant result) override;

    signals:
        void onRestData(RestReturnData **data);

    public slots:
        void fullscreenShortcut();

    private slots:
        void onRestDataFromDLL(RestReturnData *data);

    private:
        // Returns true to end processing
        bool processPageReturnAction(PageReturnAction action, QWidget **newPage);

        void popTopPage(QWidget **oldPage, QWidget **actualPage); // Pops the actual top page (Check for loading page at top of stack)
        void setPage(QWidget *page, QWidget *oldPage = nullptr);
        void deletePage(QWidget *page, QWidget *page2 = nullptr); // Protects the loading page from getting deleted
        void displayLoadingPage();

        RFIDInterface *rfidInterface;
        RESTInterface *restInterface;

        QStack<QWidget*> pageStack;
        PageLoading *loadingPage;

        Ui::ATMWindow *ui;
        QString baseTitle;
};
#endif // ATMWINDOW_H
