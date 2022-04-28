#include "pinwidget.h"
#include "ui_pinwidget.h"
#include <QTimer>

QString message;
QRegExp reNumber("^[0-9]+\\,?[0-9]{0,2}$");  // start, numbers, optional comma, maximum two numbers, end
QRegExp reComma(","); //needs work, see on_pushButton_comma_clicked()


PinWidget::PinWidget(QWidget *parent) : PinInterface(parent), ui(new Ui::PinWidget)
{
    qDebug()<<"PinWidget constructorissa";
    ui->setupUi(this);
    pinWidgetTimer.setSingleShot(true);
    pinWidgetTimer.start(10000);
    connect(&pinWidgetTimer, SIGNAL(timeout()), this, SLOT(pinWidgetIdleTimeout()));
}

PinWidget::~PinWidget()
{
    qDebug()<<"PinWidget destruktorissa";
    delete ui;
    ui=nullptr;
}

void PinWidget::pinTimerReset()
{
    qDebug()<<"pinWidgetTimer reset";
    qDebug() << PinWidget::pinWidgetTimer.remainingTime();
    PinWidget::pinWidgetTimer.start(10000);
}

void PinWidget::pinWidgetIdleTimeout()
{
    qDebug() << "pinWidgetIdleTimeout";
    emit deletePinWidget();
}

void PinWidget::setPinMessage(QString message)
{
    this->ui->pinMessage->setText(message);
}

void PinWidget::on_pushButton_1_clicked()
{
    PinWidget::pinWidgetHandleNumber(1);
}

void PinWidget::on_pushButton_2_clicked()
{
    PinWidget::pinWidgetHandleNumber(2);
}

void PinWidget::on_pushButton_3_clicked()
{
    PinWidget::pinWidgetHandleNumber(3);
}

void PinWidget::on_pushButton_4_clicked()
{
    PinWidget::pinWidgetHandleNumber(4);
}

void PinWidget::on_pushButton_5_clicked()
{
    PinWidget::pinWidgetHandleNumber(5);
}

void PinWidget::on_pushButton_6_clicked()
{
    PinWidget::pinWidgetHandleNumber(6);
}

void PinWidget::on_pushButton_7_clicked()
{
    PinWidget::pinWidgetHandleNumber(7);
}

void PinWidget::on_pushButton_8_clicked()
{
    PinWidget::pinWidgetHandleNumber(8);
}

void PinWidget::on_pushButton_9_clicked()
{
    PinWidget::pinWidgetHandleNumber(9);
}

void PinWidget::on_pushButton_0_clicked()
{
    PinWidget::pinWidgetHandleNumber(0);
}

void PinWidget::pinWidgetHandleNumber(int keyNumber)
{
    if (!reNumber.exactMatch(ui->pinMessage->text())) ui->pinMessage->clear(); //Jos viestikentässä jotain muuta kuin numeroita tyhjennetään kenttä
    ui->pinMessage->setText(ui->pinMessage->text() + QString::number(keyNumber));
    PinWidget::pinTimerReset();
}

// * disallow comma in pin, allow comma when transferring between accounts
// * print asterisks instead of numbers when typing pin

void PinWidget::on_pushButton_comma_clicked()
{
    if (!reNumber.exactMatch(ui->pinMessage->text())) ui->pinMessage->clear(); //Jos viestikentässä jotain muuta kuin numeroita tyhjennetään kenttä
    if (!reComma.exactMatch(ui->pinMessage->text())) ui->pinMessage->setText(ui->pinMessage->text() + ","); //Jos on jo pilkku niin ei sallita toista
    PinWidget::pinTimerReset();
}

void PinWidget::on_pushButton_exit_clicked()
{
    //emitsignal käyttäjä poistui?
    emit deletePinWidget();
}

void PinWidget::on_pushButton_cancel_clicked()
{
    QString tempText = ui->pinMessage->text();
    tempText.chop(1);
    ui->pinMessage->setText(tempText);
    PinWidget::pinTimerReset();
}

void PinWidget::on_pushButton_enter_clicked()
{
    QString userInput = ui->pinMessage->text();
    qDebug()<<"emitting pinWidgetUserInput \"" + userInput + "\"";
    if (reNumber.exactMatch(userInput)) emit pinWidgetUserInput(userInput);
}

