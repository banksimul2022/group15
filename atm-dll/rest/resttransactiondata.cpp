#include "resttransactiondata.h"

#include <QJsonArray>

RestTransactionData::RestTransactionData(QJsonObject *transactionInfo,
    int error):RestReturnData(RestReturnData::typeTransaction,error),transactions(new QList<BankTransaction*>)
{
    if (error != -1) {
        return;
    }

    this->count=transactionInfo->value("count").toInt(-1);

    this->hasPrev=transactionInfo->value("hasPrev").toBool(false);
    this->hasNext=transactionInfo->value("hasNext").toBool(false);

    QJsonArray transactions = transactionInfo->value("transactions").toArray();
    foreach (QJsonValue transaction,transactions) {
        this->transactions->append(new BankTransaction(transaction.toObject()));
    }

}

RestTransactionData::~RestTransactionData()
{
 qDeleteAll(*this->transactions);
 delete this->transactions;
 this->transactions = nullptr;
}

int RestTransactionData::getCount()
{

    return this->count;
}


bool RestTransactionData::getHasPrev()
{
    return this->hasPrev;
}

bool RestTransactionData::getHasNext()
{
    return this->hasNext;
}

BankTransaction *RestTransactionData::getTransaction(int id)
{
    if (id<0 || id>this->count) return nullptr;
    return this->transactions->at(id);
}


