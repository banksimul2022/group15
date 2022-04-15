#include "restinfodata.h"

RestInfoData::RestInfoData(QString fName,QString lName,QString accountNumber,int error):
    RestReturnData(RestReturnData::typeInfo,error),
    fName(fName),
    lName(lName),
    accountNumber(accountNumber)
{}

QString RestInfoData::getfName()
{
    return this->fName;
}

QString RestInfoData::getlName()
{
    return this->lName;
}

QString RestInfoData::getAccountNumber()
{
    return this->accountNumber;
}
