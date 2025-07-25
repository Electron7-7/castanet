#ifndef ARGUMENT_H
#define ARGUMENT_H

#include "common/compare_strings.hpp"
#include <string>

struct _Arg
{
public:
    constexpr _Arg(const char* LongName, const char* ShortName)
    :_long_name(LongName), _short_name(ShortName)
    {}

    constexpr _Arg(const _Arg& CopyFrom)
    : _Arg(CopyFrom._long_name, CopyFrom._short_name)
    {}

    constexpr _Arg(const char* Name)
    : _Arg(Name, Name)
    {}

    constexpr const char* LongName()  const { return _long_name;  }
    constexpr const char* ShortName() const { return _short_name; }

    // For printouts where we need to print a name and prefer to print the long name
    constexpr const char* PrettyName() const
    {
        if(CompareStrings(_long_name, ""))
            return _short_name;
        return _long_name;
    }

    constexpr bool operator==(const _Arg& other) const
    {
        if(std::string(_short_name).empty() || std::string(other._short_name).empty())
            return CompareStrings(_long_name, other._long_name);

        if(std::string(_long_name).empty() || std::string(other._long_name).empty())
            return CompareStrings(_short_name, other._short_name);

        return (CompareStrings(_long_name, other._long_name) || CompareStrings(_short_name, other._short_name));
    }

    constexpr bool operator!=(const _Arg& other) const { return !(*this == other);   }

protected:
    friend struct Option;

    const char* _long_name = "";
    const char* _short_name = "";
};

struct Flag : public _Arg
{
public:
    using _Arg::_Arg;

    void Activate() { _active = true; }
    bool IsActive() const { return _active; }

private:
    bool _active = false;
};

struct Option : public _Arg
{
public:
    constexpr Option(const char* LongName, const char* ShortName, bool IsOptionMandatory = false)
    :_Arg(LongName, ShortName), _is_option_mandatory(IsOptionMandatory)
    {}

    constexpr Option(const Option& CopyFrom)
    : _Arg(CopyFrom._long_name, CopyFrom._short_name), _value(CopyFrom._value), _is_option_mandatory(CopyFrom._is_option_mandatory)
    {}

    constexpr Option(const char* Name)
    : _Arg(Name, Name), _is_option_mandatory(false)
    {}

    bool IsOptionMandatory() const { return _is_option_mandatory; }

    void SetValue(const char* Value) { _value = Value; }
    const char* GetValue() const { return _value; }
    bool HasValue() const { return (!CompareStrings(_value, "")); } // FIXME: Kind of redundant, since I can just check that 'GetValue' isn't an empty string...

private:
    const char* _value = "";
    bool _is_option_mandatory = false;
};

#endif // ARGUMENT_H
