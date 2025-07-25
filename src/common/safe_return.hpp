#ifndef SAFE_RETURN_H
#define SAFE_RETURN_H

#include "error_codes.hpp"

template<typename T>
struct SafeReturn
{
public:
    SafeReturn(T Data, ErrCode ErrorCode = Err::SUCCESS)
    : data(Data), error_code(ErrorCode)
    {}

    ErrCode ErrorCode() const { return error_code; }
    T Data() const { return data; }

private:
    T data;
    ErrCode error_code = Err::SUCCESS;
};

#endif // SAFE_RETURN_H
