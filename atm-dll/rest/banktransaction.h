#ifndef BANKTRANSACTION_H
#define BANKTRANSACTION_H

#include "REST_global.h"

#include <QJsonObject>
#include <QDateTime>


class REST_EXPORT BankTransaction
{
public:
    BankTransaction(QJsonObject transaction);
    ~BankTransaction();
    QDateTime getTimestamp();
    QString getType();
    double getSum();
    QString getCardNumber();
    QString getAccount();
private:
     QDateTime timestamp;
     QString type;
     double sum;
     QString cardNumber;
     QString toAcount;
};

#endif // BANKTRANSACTION_H
