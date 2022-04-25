#include "pinwidget.h"
#include "ui_pinwidget.h"

QString message;
QRegExp reNumber("[0-9]*");  // a digit (\d), zero or more times (*)
QRegExp reComma(","); //needs work, see on_pushButton_comma_clicked()

PinWidget::PinWidget(QWidget *parent) : PinInterface(parent), ui(new Ui::PinWidget)
{
    qDebug()<<"PinWidget constructorissa";
    ui->setupUi(this);
}

PinWidget::~PinWidget()
{
    qDebug()<<"PinWidget destruktorissa";
    delete ui;
    ui=nullptr;
}

void PinWidget::setPinMessage(QString message)
{
    this->ui->pinMessage->setText(message);
}

void PinWidget::on_pushButton_1_clicked()
{
    if (!reNumber.exactMatch(ui->pinMessage->text())) ui->pinMessage->clear(); //Jos viestikentässä jotain muuta kuin numeroita tyhjennetään kenttä
    ui->pinMessage->setText(ui->pinMessage->text() + "1");
}

void PinWidget::on_pushButton_2_clicked()
{
    if (!reNumber.exactMatch(ui->pinMessage->text())) ui->pinMessage->clear(); //Jos viestikentässä jotain muuta kuin numeroita tyhjennetään kenttä
    ui->pinMessage->setText(ui->pinMessage->text() + "2");
}

void PinWidget::on_pushButton_3_clicked()
{
    if (!reNumber.exactMatch(ui->pinMessage->text())) ui->pinMessage->clear(); //Jos viestikentässä jotain muuta kuin numeroita tyhjennetään kenttä
    ui->pinMessage->setText(ui->pinMessage->text() + "3");
}

void PinWidget::on_pushButton_4_clicked()
{
    if (!reNumber.exactMatch(ui->pinMessage->text())) ui->pinMessage->clear(); //Jos viestikentässä jotain muuta kuin numeroita tyhjennetään kenttä
    ui->pinMessage->setText(ui->pinMessage->text() + "4");
}

void PinWidget::on_pushButton_5_clicked()
{
    if (!reNumber.exactMatch(ui->pinMessage->text())) ui->pinMessage->clear(); //Jos viestikentässä jotain muuta kuin numeroita tyhjennetään kenttä
    ui->pinMessage->setText(ui->pinMessage->text() + "5");
}

void PinWidget::on_pushButton_6_clicked()
{
    if (!reNumber.exactMatch(ui->pinMessage->text())) ui->pinMessage->clear(); //Jos viestikentässä jotain muuta kuin numeroita tyhjennetään kenttä
    ui->pinMessage->setText(ui->pinMessage->text() + "6");
}

void PinWidget::on_pushButton_7_clicked()
{
    if (!reNumber.exactMatch(ui->pinMessage->text())) ui->pinMessage->clear(); //Jos viestikentässä jotain muuta kuin numeroita tyhjennetään kenttä
    ui->pinMessage->setText(ui->pinMessage->text() + "7");
}

void PinWidget::on_pushButton_8_clicked()
{
    if (!reNumber.exactMatch(ui->pinMessage->text())) ui->pinMessage->clear(); //Jos viestikentässä jotain muuta kuin numeroita tyhjennetään kenttä
    ui->pinMessage->setText(ui->pinMessage->text() + "8");
}

void PinWidget::on_pushButton_9_clicked()
{
    if (!reNumber.exactMatch(ui->pinMessage->text())) ui->pinMessage->clear(); //Jos viestikentässä jotain muuta kuin numeroita tyhjennetään kenttä
    ui->pinMessage->setText(ui->pinMessage->text() + "9");
}

void PinWidget::on_pushButton_0_clicked()
{
    if (!reNumber.exactMatch(ui->pinMessage->text())) ui->pinMessage->clear(); //Jos viestikentässä jotain muuta kuin numeroita tyhjennetään kenttä
    ui->pinMessage->setText(ui->pinMessage->text() + "0");
}

/*
 * needs to be handled, allow/disallow comma in pin, allow comma when
 * transferring between accounts, handle comma in message window

void PinWidget::on_pushButton_comma_clicked()
{
    if (!reNumber.exactMatch(ui->pinMessage->text())) ui->pinMessage->clear(); //Jos viestikentässä jotain muuta kuin numeroita tyhjennetään kenttä
    if (!reComma.exactMatch(ui->pinMessage->text())) ui->pinMessage->setText(ui->pinMessage->text() + ","); //Jos on jo pilkku niin ei sallita toista
}
*/

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
}

void PinWidget::on_pushButton_enter_clicked()
{
    QString userInput = ui->pinMessage->text();
    qDebug()<<"emitting pinWidgetUserInput \"" + userInput + "\"";
    if (reNumber.exactMatch(userInput)) emit pinWidgetUserInput(userInput);
}

