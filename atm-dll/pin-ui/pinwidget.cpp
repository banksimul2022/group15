#include "pinwidget.h"
#include "ui_pinwidget.h"
#include <QTimer>

QString message;
QRegExp reNumber("^[0-9]+,?[0-9]{0,2}$");  // start, numbers, optional comma, maximum two numbers, end
QRegExp reHasTwoDecimals(",[0-9][0-9]$");
QRegExp reBeginsWithComma("^[,]");

PinWidget::PinWidget(QWidget *parent) : PinInterface(parent), ui(new Ui::PinWidget)
{
    qDebug()<<"PinWidget constructorissa";
    ui->setupUi(this);
    pinWidgetTimer.setSingleShot(true);
    connect(&pinWidgetTimer, SIGNAL(timeout()), this, SLOT(pinWidgetIdleTimeout()));
    pinTimerReset();
    QTimer *updater = new QTimer;
    connect(updater, &QTimer::timeout, this, &PinWidget::updatePinWidgetTimerBar);
    updater->start(100);
}

PinWidget::~PinWidget()
{
    qDebug()<<"PinWidget destruktorissa";
    delete ui;
    ui=nullptr;
}

void PinWidget::pinTimerReset()
{
    qDebug()<< "pinWidgetTimer reset at" << PinWidget::pinWidgetTimer.remainingTime() << " ms";
    PinWidget::pinWidgetTimer.start(10000);
}

void PinWidget::updatePinWidgetTimerBar()
{
    ui->timerBar->setValue(pinWidgetTimer.remainingTime());
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
    PinWidget::pinTimerReset();
    if (!reNumber.exactMatch(ui->pinMessage->text()))
        ui->pinMessage->clear(); //Jos viestikentässä jotain muuta kuin numeroita tyhjennetään kenttä
    if (!ui->pinMessage->text().contains(reHasTwoDecimals))
        ui->pinMessage->setText(ui->pinMessage->text() + QString::number(keyNumber)); //sallitaan vain kaksi desimaalia
}

// * disallow comma in pin, allow comma when transferring between accounts
// * print asterisks instead of numbers when typing pin

void PinWidget::on_pushButton_comma_clicked()
{
    PinWidget::pinTimerReset();
    if (!reNumber.exactMatch(ui->pinMessage->text()))
        ui->pinMessage->clear(); //Jos viestikentässä jotain muuta kuin numeroita tyhjennetään kenttä
    if (!ui->pinMessage->text().contains(",")&&ui->pinMessage->text()!="")
        ui->pinMessage->setText(ui->pinMessage->text() + ","); //Jos tyhjä tai on jo pilkku niin ei sallita pilkkua
}

void PinWidget::on_pushButton_exit_clicked()
{
    //emitsignal käyttäjä poistui?
    emit deletePinWidget();
}

void PinWidget::on_pushButton_cancel_clicked()
{
    PinWidget::pinTimerReset();
    QString tempText = ui->pinMessage->text();
    tempText.chop(1);
    ui->pinMessage->setText(tempText);
}

void PinWidget::on_pushButton_enter_clicked()
{
    PinWidget::pinTimerReset();
    QString userInput = ui->pinMessage->text();
    if (reNumber.exactMatch(userInput))
    {
        emit pinWidgetUserInput(userInput); //lähetetään vain numeroita, pilkut sallittu, myös 0 sallittu huom tilisiirto
        qDebug()<<"emitting pinWidgetUserInput \"" + userInput + "\"";
    }
}

