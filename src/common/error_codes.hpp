#ifndef ERROR_CODES_H
#define ERROR_CODES_H

typedef unsigned short ErrCode;

namespace Err
{
    constexpr ErrCode SUCCESS = 0x0;

    namespace Args
    {
        constexpr ErrCode ARGUMENT_OPTION_MISSING = 0x1; // An 'option' argument flag is missing its option
        constexpr ErrCode MANDATORY_ARGUMENT_OPTION_MISSING = 0x2; // A mandatory 'option' argument flag is missing its option
        constexpr ErrCode INVALID_NUMBER_OF_HOSTS = 0x3;
    }

    namespace Config
    {
        constexpr ErrCode NO_CONFIG_FOUND = 0x1;
    }
}

#endif // ERROR_CODES_H
