#include "restbalancedata.h"


RestBalanceData::RestBalanceData(QJsonObject *balance, int error):
    RestReturnData(RestReturnData::typeBalance,error)
{
    this->balance=balance->value("balance").toDouble();
    this->credit=balance->value("credit").toDouble();
}

double RestBalanceData::getBalance()
{
    return this->balance;
}

double RestBalanceData::getCredit()
{
    return this->credit;
}
