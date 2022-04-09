#ifndef ATMWINDOW_H
#define ATMWINDOW_H

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

        void logout() override;
        void setPage(QWidget *page) override;

    public slots:
        void fullscreenShortcut();

    private:
        QWidget *currentPage;
        Ui::ATMWindow *ui;
        QString baseTitle;
};
#endif // ATMWINDOW_H
