#ifndef ARGUMENT_HANDLERS_H
#define ARGUMENT_HANDLERS_H

#include "common/error_codes.hpp"
#include "arguments/argument.hpp"
#include <vector>

ErrCode OptionsHandler(std::vector<Option>* Options);
ErrCode FlagsHandler(std::vector<Flag>* Flags);

ErrCode helper_WarnIfOptionMissingValue(Option& Option, const char* DefaultValue);
bool try_SetOutputFile(const char* WishOutputFile, bool SuppressPrintouts = false);

#endif // ARGUMENT_HANDLERS_H
