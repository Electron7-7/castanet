#ifndef ARGUMENTS_PARSER_H
#define ARGUMENTS_PARSER_H

#include "argument.hpp"
#include "common/error_codes.hpp"
#include <vector>

class ArgumentsParser
{
public:
    static void AddOption(const Option& Option);
    static void AddFlag(const Flag& Flag);
    static std::vector<Option>* GetOptions();
    static std::vector<Flag>* GetFlags();
    static ErrCode ParseArguments(int NumberOfArguments, char** Arguments);

private:
    static std::vector<Option> _options;
    static std::vector<Flag> _flags;

    static std::vector<Option> _valid_options;
    static std::vector<Flag> _valid_flags;

    static std::vector<Option> _mandatory_options;
};

extern ArgumentsParser* global_ArgumentsParser;

#endif // ARGUMENTS_PARSER_H
