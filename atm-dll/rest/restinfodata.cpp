#include "restinfodata.h"
#include <QImageReader>

RestInfoData::RestInfoData(QJsonObject *info,QIODevice *imageDevice,int error,int imgError):
    RestReturnData(RestReturnData::typeInfo,error),imgError(imgError)

{
    this->fName=info->value("fName").toString();
    this->lName=info->value("lName").toString();
    this->accountNumber=info->value("accountNumber").toString();
    this->canUseCredit=info->value("credit").toBool(false);


    QImageReader reader(imageDevice);
    this->profile = QPixmap::fromImageReader(&reader);


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

bool RestInfoData::credit()
{
    return this->canUseCredit;
}

int RestInfoData::getImgError()
{
    return this->imgError;
}

QPixmap RestInfoData::getProfile()
{
   return this->profile;
}


