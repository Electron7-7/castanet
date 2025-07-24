#ifndef ARGUMENT_VARIABLES_H
#define ARGUMENT_VARIABLES_H

#include <string>

inline bool flag_DebugMode = false;
inline bool flag_NoColor = false;
inline std::string option_OutputFile = "castanet_output";

// Contants related to the configuration file
constexpr const char* constant_ConfigFileLocationUserHome = "~/.castanet_config";
constexpr const char* constant_ConfigFileLocationConfigDir = "~/.config/castanet_confing";
constexpr const char* constant_ConfigFileLocationEnvironmentVariable = "$CASTANET_CONFIG";

// I think '~/.castanet_config' is a good default value, should all else fail
inline std::string option_ConfigFile = constant_ConfigFileLocationUserHome;

#endif // ARGUMENT_VARIABLES_H
