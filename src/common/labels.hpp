#ifndef LABELS_H
#define LABELS_H

#include "colors.hpp"
#include "system/argument_variables.hpp"

#define LABEL_ERROR "[ERROR]"
#define LABEL_WARN  "[WARNING]"
#define LABEL_DEBUG "[DEBUG]"

static const char* ERROR()
{
    if(flag_NoColor)
        return LABEL_ERROR;
    return COLOR_BOLD(RED) LABEL_ERROR;
}

static const char* WARN()
{
    if(flag_NoColor)
        return LABEL_WARN;
    return COLOR_BOLD(YELLOW) LABEL_WARN;
}

static const char* DEBUG()
{
    if(flag_NoColor)
        return LABEL_DEBUG;
    return COLOR_BOLD(DEFAULT) LABEL_DEBUG COLOR_RESET;
}

static const char* RESET_COLOR()
{
    if(flag_NoColor)
        return "";
    return COLOR_RESET;
}

#endif // LABELS_H
