#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "pininterface.h"
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    PinInterface *pPinInterface;
    Ui::MainWindow *ui;

signals:
    void PinUiShowMessage(QString message);

private slots:
    void on_btnOpenPinWidget_clicked();
    void on_btnCorrectPIN_clicked();
    void on_btnWrongPIN_clicked();

public slots:
    void updateMainWindowLineEdit(QString userInput);
    void deletePinWidget();
};
#endif // MAINWINDOW_H
