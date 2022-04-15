#ifndef RESTINFODATA_H
#define RESTINFODATA_H
#include <QString>
#include "restreturndata.h"

class REST_EXPORT RestInfoData : public RestReturnData
{
public:
    RestInfoData(QString fName,QString lName,QString accountNumber,int error);
    QString getfName();
    QString getlName();
    QString getAccountNumber();



private:
    const QString fName;
    const QString lName;
    const QString accountNumber;
};

#endif // RESTINFODATA_H
