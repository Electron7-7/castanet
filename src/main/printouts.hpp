#ifndef PRINTOUTS_H
#define PRINTOUTS_H

#include "arguments.hpp"
#include <print> // IWYU pragma: keep

inline bool ALLOW_PRINTOUT() { return (!Flags::Silent.IsActive() && !Flags::DryRun.IsActive()); }
inline bool ALLOW_MESSAGE()  { return (ALLOW_PRINTOUT() && !Flags::NoMessage.IsActive()); }

#define PRINT_OUT(FORMAT, ARGS...) if(ALLOW_PRINTOUT()) std::print(FORMAT, ARGS);
#define PRINTOUT(FORMAT) PRINT_OUT(FORMAT,"")

#define PRINT_MESSAGE(FORMAT, ARGS...) if(ALLOW_MESSAGE()) std::print(FORMAT, ARGS);

#define PRINT_DEBUG(FORMAT, ARGS...) if(Flags::DebugMode.IsActive()) std::print("\x1b[1;39m[DEBUG]\x1b[0m " FORMAT, ARGS);
#define PRINTDEBUG(FORMAT) PRINT_DEBUG("{}", FORMAT)

#endif // PRINTOUTS_H
