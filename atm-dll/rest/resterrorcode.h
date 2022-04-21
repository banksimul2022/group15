#ifndef RESTERRORCODE_H
#define RESTERRORCODE_H
#define REST_CREATE_ERR_CODE(type, code) (type << 16) | code
#include <qglobal.h>

namespace RestErrors {
enum types:ushort {
    ERR_GENERIC = 1 << 0, // Generic errors
        ERR_VALUE  = 1 << 1, // Errors pertaining to values
        ERR_PERMISSION = 1 << 2  // Errors pertaining to permissions

};

enum errors:uint {
    ERR_UNKNOWN = REST_CREATE_ERR_CODE(ERR_GENERIC, 1),
        ERR_UNKNOWN_PERM_FLAG = REST_CREATE_ERR_CODE(ERR_PERMISSION, 2),

        ERR_DATABASE = REST_CREATE_ERR_CODE(ERR_GENERIC, 3),

        ERR_INVALID_PARAM = REST_CREATE_ERR_CODE(ERR_VALUE, 4),

        ERR_INVALID_CREDENTIALS = REST_CREATE_ERR_CODE(ERR_VALUE, 5),
        ERR_INVALID_AUTH = REST_CREATE_ERR_CODE(ERR_VALUE | ERR_PERMISSION ,6),
        ERR_NOT_ALLOWED = REST_CREATE_ERR_CODE(ERR_PERMISSION, 7),

        ERR_INSUFFICIENT_FUNDS = REST_CREATE_ERR_CODE(ERR_VALUE, 8),
        ERR_INVALID_SUM = REST_CREATE_ERR_CODE(ERR_VALUE, 9)

};

enum internalErrors:int {
    ERR_UNTAGGED_REPLY,ERR_NETWORK,ERR_UNKNOW_REPLY,ERR_INVALID_STATE

};

}
#endif // RESTERRORCODE_H

