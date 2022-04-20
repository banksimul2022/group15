#ifndef RESTBALANCEDATA_H
#define RESTBALANCEDATA_H

#include "restreturndata.h"
#include <QJsonObject>

class REST_EXPORT RestBalanceData : public RestReturnData
{
public:
    RestBalanceData(QJsonObject *balance,int error);
    double getBalance();
    double getCredit();

private:
    double balance;
    double credit;

};

#endif // RESTBALANCEDATA_H
