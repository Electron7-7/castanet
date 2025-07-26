#ifndef ARGUMENT_VARIABLES_H
#define ARGUMENT_VARIABLES_H

#include <string>

inline bool flag_DebugMode = false;
inline bool flag_DebugDry = false;
inline bool flag_DebugAll = false;

inline std::string option_OutputFile = "castanet_output";

// Contants related to the configuration file
constexpr const char* constant_ConfigFileLocationUserHome = "~/.castanet_config";
constexpr const char* constant_ConfigFileLocationConfigDir = "~/.config/castanet_confing";
constexpr const char* constant_ConfigFileLocationEnvironmentVariable = "CASTANET_OUTPUT";

// I think '~/.castanet_config' is a good default value, should all else fail
inline std::string option_ConfigFile = constant_ConfigFileLocationUserHome;
inline bool option_Output_WasSpecified = false;

inline bool flag_NoColor = false;
inline bool flag_NoMessage = false;
inline bool flag_Silent = false;
inline bool flag_Minimal = false;
inline bool flag_Verbose = false;
inline bool flag_AddTimestamp = false;
inline bool flag_IncludeDNS = false;
inline bool flag_Pipe = false;

inline long long argument_NumberOfHosts = 0;

inline int last_argc_processed = 0;

#endif // ARGUMENT_VARIABLES_H
