#include "arguments_parser.hpp"
#include "common/labels.hpp"
#include <cstdio>
#include <stdexcept>
#include <string>

ArgumentsParser static_ArgumentsParser;
ArgumentsParser* global_ArgumentsParser = &static_ArgumentsParser;

std::vector<Option> ArgumentsParser::_options = {};
std::vector<Flag> ArgumentsParser::_flags = {};
std::vector<Option> ArgumentsParser::_valid_options = {};
std::vector<Flag> ArgumentsParser::_valid_flags = {};
std::vector<Option> ArgumentsParser::_mandatory_options = {};

bool _contains(std::vector<Option>& options, const Option& option)
{
    for(Option& _option : options)
        if(option == _option)
            return true;
    return false;
}

bool _contains(std::vector<Flag>& flags, const Flag& flag)
{
    for(Flag& _flag : flags)
        if(flag == _flag)
            return true;
    return false;
}

void ArgumentsParser::AddOption(const Option& option)
{
    if(_contains(_valid_options, option))
        return;

    _valid_options.insert(_valid_options.end(), option);
}

void ArgumentsParser::AddFlag(const Flag& flag)
{
    if(_contains(_valid_flags, flag))
        return;

    _valid_flags.insert(_valid_flags.end(), flag);
}

std::vector<Option>* ArgumentsParser::GetOptions()
{ return &_options; }

std::vector<Flag>* ArgumentsParser::GetFlags()
{ return &_flags; }

ErrCode ArgumentsParser::ParseArguments(int argc, char** argv)
{
    for(unsigned int i = 1 ; i < argc ; i++)
    {
        for(unsigned int i_f = 0 ; i_f < _valid_flags.size() ; i_f++)
        {
            if(_valid_flags.at(i_f) == argv[i])
            {
                if(last_argc_processed < i)
                    last_argc_processed = i;

                _valid_flags.at(i_f).Activate();
                _flags.insert(_flags.end(), _valid_flags.at(i_f));
                continue;
            }
        }

        for(unsigned int i_o = 0 ; i_o < _valid_options.size() ; i_o++)
        {
            if(_valid_options.at(i_o) == argv[i])
            {
                if((i+1) < argc) _valid_options.at(i_o).SetValue(argv[++i]);
                _options.insert(_options.end(), _valid_options.at(i_o));

                if(last_argc_processed < i)
                    last_argc_processed = i;

                if(_options.at(i_o).IsOptionMandatory())
                    _mandatory_options.insert(_mandatory_options.end(), _valid_options.at(i_o));
                continue;
            }
        }
    }

    for(const Option& option : _mandatory_options)
    {
        if(!option.HasValue())
        {
            if(!flag_Silent && !flag_NoMessage)
                printf("%s ArgumentsParser::ParseArguments - The argument '%s' requires a valid option but none were given!%s\n", ERROR(), option.PrettyName(), COLOR_RESET);
            return Err::Args::MANDATORY_ARGUMENT_OPTION_MISSING;
        }
    }

    _mandatory_options.clear();
    _valid_options.clear();
    _valid_flags.clear();

    return Err::SUCCESS;
}
