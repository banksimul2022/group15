#include "restreturndata.h"

RestReturnData::RestReturnData(ReturnType type, int error):dataType(type),errorCode(error)
{}

RestReturnData::ReturnType RestReturnData::type()
{
    return this->dataType;
}

int RestReturnData::error()
{
    return this->errorCode;
}
