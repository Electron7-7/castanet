#ifndef LABELS_H
#define LABELS_H

#include "colors.hpp"
#include "system/arguments.hpp"

#define LABEL_ERROR "[ERROR]"
#define LABEL_WARN  "[WARNING]"
#define LABEL_DEBUG "[DEBUG]"

static const char* ERROR()
{
    if(Flags::NoColor.IsActive())
        return LABEL_ERROR;
    return COLOR_BOLD(RED) LABEL_ERROR;
}

static const char* WARN()
{
    if(Flags::NoColor.IsActive())
        return LABEL_WARN;
    return COLOR_BOLD(YELLOW) LABEL_WARN;
}

static const char* DEBUG()
{
    if(Flags::NoColor.IsActive())
        return LABEL_DEBUG;
    return COLOR_BOLD(DEFAULT) LABEL_DEBUG COLOR_RESET;
}

static const char* RESET_COLOR()
{
    if(Flags::NoColor.IsActive())
        return "";
    return COLOR_RESET;
}

#endif // LABELS_H
