#ifndef RESTRETURNDATA_H
#define RESTRETURNDATA_H
#include "REST_global.h"

class REST_EXPORT RestReturnData
{
public:
    enum ReturnType : int {
        typeLogin,typeLogout,typeInfo //login =0 / logout 1
    };
    RestReturnData(ReturnType type,int error = -1);

    ReturnType type();
    int error();
private:
   const ReturnType dataType;
   const int errorCode;

};

#endif // RESTRETURNDATA_H
