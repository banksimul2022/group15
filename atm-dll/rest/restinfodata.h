#ifndef RESTINFODATA_H
#define RESTINFODATA_H
#include "restreturndata.h"
#include <QPixmap>


#include <QJsonObject>


class REST_EXPORT RestInfoData : public RestReturnData
{

public:
    RestInfoData(QJsonObject *info,QIODevice *imageDevice,int error,int imgError);
    QString getfName();
    QString getlName();
    QString getAccountNumber();
    bool credit();
    int getImgError();
    QPixmap getProfile();




private:
     QString fName;
     QString lName;
     QString accountNumber;
     bool canUseCredit;
     QPixmap profile;
     const int imgError;
};

#endif // RESTINFODATA_H
