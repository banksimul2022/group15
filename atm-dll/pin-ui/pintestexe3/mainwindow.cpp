#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pininterface.h"

bool pindebug=true;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      pPinInterface(nullptr),
      ui(new Ui::MainWindow)
{
    if (pindebug) qDebug() << "MainWindow konstruktorissa";
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    if (pindebug) qDebug() << "MainWindow destruktorissa";
    delete ui;
    ui=nullptr;
}

void MainWindow::updateMainWindowLineEdit(QString userInput)
{
    qDebug()<<"updateLineEdit kutsuttu " + userInput;
    this->ui->MainWindowLineEdit->setText(userInput);
    if(this->pPinInterface == nullptr) return;
}

void MainWindow::on_btnOpenPinWidget_clicked()
{
    if (pindebug) qDebug()<<"PIN test exe button OPEN PIN WIDGET clicked";
    if(this->pPinInterface != nullptr) {
        if (pindebug) qDebug()<<"PIN widget pointer is not NULL, not opening widget";
        return;
    }
    this->pPinInterface = PinInterface::createWidgetInstance(this);
    this->ui->widgetLayout->addWidget(this->pPinInterface);

    this->connect(this->pPinInterface, &PinInterface::pinWidgetUserInput,
                  this, &MainWindow::updateMainWindowLineEdit); //send pin to test exe

    this->connect(this, &MainWindow::PinUiShowMessage,
                  this->pPinInterface, &PinInterface::setPinMessage); //send message to PinWidget PinMessage lineEdit

    this->connect(this->pPinInterface, &PinInterface::deletePinWidget,
                  this, &MainWindow::deletePinWidget); //delete widget and set pointers to nullptr
}


void MainWindow::on_btnCorrectPIN_clicked()
{
    if (pindebug) qDebug()<<"PIN test exe button CORRECT PIN clicked";
    if(this->pPinInterface != nullptr)
    {
        this->ui->widgetLayout->removeWidget(this->pPinInterface);
        this->pPinInterface->setParent(nullptr);
        delete this->pPinInterface;
        this->pPinInterface = nullptr;
        ui->MainWindowLineEdit->setText("Pin widgetti suljettu");
    }
}

void MainWindow::on_btnWrongPIN_clicked()
{
    if (pindebug) qDebug()<<"PIN test exe button WRONG PIN clicked";
    emit PinUiShowMessage("Wrong PIN!");
}

void MainWindow::deletePinWidget() { // on_btnEXIT_clicked()
    if(this->pPinInterface != nullptr) {
        if (pindebug) qDebug()<<"deleting Pin Widget";
        this->ui->widgetLayout->removeWidget(this->pPinInterface);
        this->pPinInterface->setParent(nullptr);
        delete this->pPinInterface;
        this->pPinInterface = nullptr;
    }
}
