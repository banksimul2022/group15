#include "restinfodata.h"

RestInfoData::RestInfoData(QJsonObject *info,int error):
    RestReturnData(RestReturnData::typeInfo,error)

{
    this->fName=info->value("fName").toString();
    this->lName=info->value("lName").toString();
    this->accountNumber=info->value("accountNumber").toString();

}

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
