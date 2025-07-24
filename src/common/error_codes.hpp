#ifndef ERROR_CODES_H
#define ERROR_CODES_H

typedef unsigned short ErrCode;

namespace Err
{
    constexpr unsigned short NO_ERROR = 0x0;

    namespace Args
    {
        constexpr unsigned short ARGUMENT_OPTION_MISSING = 0x1; // An 'option' argument flag is missing its option
        constexpr unsigned short MANDATORY_ARGUMENT_OPTION_MISSING = 0x2; // A mandatory 'option' argument flag is missing its option
    }

    namespace Generator
    {
        constexpr unsigned short SOURCE_DIR_INVALID = 0x2;
    }
}

template<typename T>
struct SafeReturn
{
public:
    SafeReturn(T Data, ErrCode ErrorCode = Err::NO_ERROR)
    : data(Data), error_code(ErrorCode)
    {}

    ErrCode ErrorCode() const { return error_code; }
    T Data() const { return data; }

private:
    T data;
    ErrCode error_code = Err::NO_ERROR;
};

#endif // ERROR_CODES_H
