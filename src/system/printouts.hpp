#ifndef PRINTOUTS_H
#define PRINTOUTS_H

inline bool ALLOW_PRINTOUT() { return (!Flags::Silent.IsActive() && !Flags::DryRun.IsActive()); }
inline bool ALLOW_MESSAGE()  { return (ALLOW_PRINTOUT() && !Flags::NoMessage.IsActive()); }

#define PRINT_OUT(FORMAT, ARGS...) if(ALLOW_PRINTOUT()) std::print(FORMAT, ARGS);
#define PRINTOUT(FORMAT) PRINT_OUT(FORMAT,"")
#define PRINT_MESSAGE(FORMAT, ARGS...) if(ALLOW_MESSAGE()) std::print(FORMAT, ARGS);

#endif // PRINTOUTS_H
