#ifndef ARGUMENT_H
#define ARGUMENT_H

constexpr bool CompareStrings(const char* Left, const char* Right)
{
    if(Left == Right) // Hey, it could happen
        return true;

    if(Left == nullptr || Right == nullptr) // All my homies HATE segfaults!!!!
        return false;

    while(*Left != '\0' && *Right != '\0')
    {
        if(*Left != *Right)
            return false;

        Left++;
        Right++;
    }

    if(*Left != '\0' || *Right != '\0') // One of them is shorter than the other!
        return false;

    return true;
}

struct _Arg
{
public:
    constexpr _Arg(const char* LongName, const char* ShortName)
    : _long_name(LongName), _short_name(ShortName), _active(false)
    {}

    constexpr _Arg(const _Arg& CopyFrom)
    : _Arg(CopyFrom._long_name, CopyFrom._short_name)
    {}

    constexpr _Arg(const char* Name)
    : _Arg(Name, Name)
    {}

    constexpr bool IsActive() const { return _active; }
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
        // FIXME: Replace '.empty()' with 'CompareStrings()' and an empty string
        if(CompareStrings(_short_name, "") || CompareStrings(other._short_name, ""))
            return CompareStrings(_long_name, other._long_name);

        if(CompareStrings(_long_name, "") || CompareStrings(other._long_name, ""))
            return CompareStrings(_short_name, other._short_name);

        return (CompareStrings(_long_name, other._long_name) || CompareStrings(_short_name, other._short_name));
    }

    constexpr bool operator!=(const _Arg& other) const { return !(*this == other); }

    constexpr bool operator==(const char* string) const
    { return (CompareStrings(_short_name, string) || CompareStrings(_long_name, string)); }

    constexpr bool operator!=(const char* string) const
    { return !(*this == string); }

protected:
    friend struct Option;

    const char* _long_name = "";
    const char* _short_name = "";
    bool _active = false;
};

struct Flag : public _Arg
{
public:
    using _Arg::_Arg;

    void Activate() { _active = true; }
    void Deactivate() { _active = false; }
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

    constexpr Option(const char* Name, bool IsOptionMandatory = false)
    : Option(Name, Name, IsOptionMandatory)
    {}

    void SetValue(const char* Value)
    {
        _value = Value;
        _active = true;
    }

    void EraseValue()
    {
        _value = "";
        _active = false;
    }

    bool HasValue() const { return (!CompareStrings(_value, "")); } // FIXME: Kind of redundant, since I can just check that 'GetValue' isn't an empty string...
    const char* GetValue() const { return _value; }
    bool IsOptionMandatory() const { return _is_option_mandatory; }

private:
    const char* _value = "";
    bool _is_option_mandatory = false;
};

#endif // ARGUMENT_H
