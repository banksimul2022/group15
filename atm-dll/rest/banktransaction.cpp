#include "banktransaction.h"


BankTransaction::BankTransaction(QJsonObject transaction)
{
    this->type=transaction.value("type").toString();
    this->sum=transaction.value("sum").toDouble(-1);
    this->cardNumber=transaction.value("cardNumber").toString(nullptr);
    this->toAcount=transaction.value("accountNumber").toString(nullptr);

    this->timestamp=QDateTime::fromString(transaction.value("timestamp").toString(),Qt::ISODate);
}

BankTransaction::~BankTransaction()
{
    qDebug()<<"tuhotaan transactio";

}

QDateTime BankTransaction::getTimestamp()
{

    return this->timestamp;
}

QString BankTransaction::getType()
{
    return this->type;
}

double BankTransaction::getSum()
{
    return this->sum;
}

QString BankTransaction::getCardNumber()
{
    return this->cardNumber;
}

QString BankTransaction::getAccount()
{
    return this->toAcount;
}
