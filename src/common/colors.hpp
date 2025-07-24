#ifndef COLORS_H
#define COLORS_H

#define BLACK   "30"
#define RED     "31"
#define GREEN   "32"
#define YELLOW  "33"
#define BLUE    "34"
#define MAGENTA "35"
#define CYAN    "36"
#define WHITE   "37"
#define DEFAULT "39"

#define COLOR_RESET "\x1b[0m"
#define COLOR(color) "\x1b[" color "m"
#define COLOR_BOLD(color) "\x1b[1;" color "m"

#endif // COLORS_H
