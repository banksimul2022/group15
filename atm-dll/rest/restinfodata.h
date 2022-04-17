#ifndef RESTINFODATA_H
#define RESTINFODATA_H
#include "restreturndata.h"

#include <QJsonObject>

class REST_EXPORT RestInfoData : public RestReturnData
{
public:
    RestInfoData(QJsonObject *info,int error);
    QString getfName();
    QString getlName();
    QString getAccountNumber();



private:
     QString fName;
     QString lName;
     QString accountNumber;
};

#endif // RESTINFODATA_H
