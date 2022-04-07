#ifndef ATMWINDOW_H
#define ATMWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class ATMWindow; }
QT_END_NAMESPACE

class ATMWindow : public QMainWindow {
    Q_OBJECT

    public:
        ATMWindow(QWidget *parent = nullptr);
        ~ATMWindow();

    public slots:
        void fullscreenShortcut();

    private:
        void setPage(QWidget *page);

        QWidget *currentPage;
        Ui::ATMWindow *ui;
        QString baseTitle;
};
#endif // ATMWINDOW_H
