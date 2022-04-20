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

        void navigateToPage(QWidget *page) override;
        bool leaveCurrentPage(QVariant result) override;

    public slots:
        void fullscreenShortcut();

    private:
        void setPage(QWidget *page, QWidget *oldPage = nullptr);

        RFIDInterface *rfidInterface;
        RESTInterface *restInterface;

        QStack<QWidget*> pageStack;
        PageLoading *loadingPage;

        Ui::ATMWindow *ui;
        QString baseTitle;
};
#endif // ATMWINDOW_H
