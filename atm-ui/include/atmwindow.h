#ifndef ATMWINDOW_H
#define ATMWINDOW_H

#include "page/pageloading.h"
#include "statemanager.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class ATMWindow; }
QT_END_NAMESPACE

class ATMWindow : public QMainWindow, public StateManager {
    Q_OBJECT

    public:
        ATMWindow(QWidget *parent = nullptr);
        ~ATMWindow();

        RFIDInterface *getRFIDInterface() override;
        RESTInterface *getRESTInterface() override;

    public slots:
        void fullscreenShortcut();

    private:
        void setPage(QWidget *page);

        RFIDInterface *rfidInterface;
        RESTInterface *restInterface;

        PageLoading *loadingPage;
        QWidget *currentPage;
        Ui::ATMWindow *ui;
        QString baseTitle;
};
#endif // ATMWINDOW_H
