#ifndef RESTTRANSACTIONDATA_H
#define RESTTRANSACTIONDATA_H
#include "restreturndata.h"
#include "banktransaction.h"
#include <QJsonObject>
#include <QList>




class REST_EXPORT RestTransactionData : public RestReturnData
{
public:
    explicit RestTransactionData(QJsonObject *transactionInfo,int error);
    ~RestTransactionData();
    int getCount();
    BankTransaction *getTransaction(int id);

    bool getHasPrev();
    bool getHasNext();

private:
    QList<BankTransaction*> *transactions;

    int count;    
    bool hasPrev;
    bool hasNext;

};

#endif // RESTTRANSACTIONDATA_H
