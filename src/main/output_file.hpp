#ifndef FILE_HANDLING_H
#define FILE_HANDLING_H

#include <string>

enum class FileStatus
{
    FAILURE              = 0b00,
    SUCCESS_FILE_EXISTS  = 0b01,
    SUCCESS_FILE_CREATED = 0b10,
};

FileStatus CheckFilePath(const std::string& WishFileName = "");
bool try_SetOutputFile(const char* WishOutputFile);

constexpr const char* constant_ConfigFileLocationEnvironmentVariable = "CASTANET_OUTPUT";
constexpr const char* constant_DefaultOutputFile = "castanet_output";

#endif // FILE_HANDLING_H
