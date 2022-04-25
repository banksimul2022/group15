class APIError extends Error {
    constructor(message, code, status) {
        super(message);
        this.code = code;
        this.status = status;
    }
}

// Used when the error details should not be sent to the requester
class PrivateAPIError extends APIError {
    constructor(message, code, status) {
        super(message, code, status);
    }
}

// Used when the error details can be sent to the requester
class PublicAPIError extends APIError {
    constructor(message, code, status) {
        super(message, code, status);
    }
}

class PermissionError extends PublicAPIError {
    constructor(message, code) {
        super(message, code, 403);
    }
}

// The error code is a unsgined 32-bit int
// The 16 top bits are individual flags indicating the error type
// The 16 bottom bits are an unsgined short representing the actual error

const types = {
    ERR_GENERIC:                 1 << 0, // Generic errors
    ERR_VALUE:                   1 << 1, // Errors pertaining to values
    ERR_PERMISSION:              1 << 2  // Errors pertaining to permissions
};

let errorCounter = 1;
const createCode = (type) => {
    return (type << 16) | errorCounter++;
};

const codes = {
    ERR_UNKNOWN: createCode(types.ERR_GENERIC),
    ERR_UNKNOWN_PERM_FLAG: createCode(types.ERR_PERMISSION),

    ERR_DATABASE: createCode(types.ERR_GENERIC),

    ERR_INVALID_PARAM: createCode(types.ERR_VALUE),

    ERR_INVALID_CREDENTIALS: createCode(types.ERR_VALUE),
    ERR_INVALID_AUTH: createCode(types.ERR_VALUE | types.ERR_PERMISSION),
    ERR_NOT_ALLOWED: createCode(types.ERR_PERMISSION),

    ERR_INSUFFICIENT_FUNDS: createCode(types.ERR_VALUE),
    ERR_INVALID_SUM: createCode(types.ERR_VALUE),
    ERR_CREDIT_NOT_SUPPORTED: createCode(types.ERR_PERMISSION),

    ERR_CARD_LOCKED: createCode(types.ERR_PERMISSION)
};

module.exports = Object.freeze({
    types,
    codes,

    PrivateAPIError,
    PublicAPIError,
    PermissionError
});