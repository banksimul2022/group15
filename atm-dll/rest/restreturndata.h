#ifndef RESTRETURNDATA_H
#define RESTRETURNDATA_H
#include "REST_global.h"

#include <QString>

class REST_EXPORT RestReturnData
{

public:
    enum ReturnType : int {
        typeLogin,typeLogout,typeInfo,typeTransaction,typeLatestTransaction,typeWithdraw,typeDeposit,typeBalance,typeinternalerror //login =0 / logout 1 jne...
    };
    RestReturnData(ReturnType type,int error = -1);


    ReturnType type();
    int error();



private:
   const ReturnType dataType;
   const int errorCode;


};

#endif // RESTRETURNDATA_H
