#ifndef COLORS_H
#define COLORS_H

#include <string>

constexpr const char* BLACK   = "30";
constexpr const char* RED     = "31";
constexpr const char* GREEN   = "32";
constexpr const char* YELLOW  = "33";
constexpr const char* BLUE    = "34";
constexpr const char* MAGENTA = "35";
constexpr const char* CYAN    = "36";
constexpr const char* WHITE   = "37";
constexpr const char* DEFAULT = "39";

constexpr const char* COLOR_RESET = "\x1b[0m";

constexpr std::string COLOR(std::string Color)
{ return "\x1b[" + Color + "m"; }

constexpr std::string COLOR_BOLD(std::string Color)
{ return "\x1b[1;" + Color + "m"; }

#endif // COLORS_H
