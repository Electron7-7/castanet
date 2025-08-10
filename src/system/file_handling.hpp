#ifndef FILE_HANDLING_H
#define FILE_HANDLING_H

extern bool try_SetOutputFile(const char* WishOutputFile, bool SuppressPrintouts = false);

enum class FileStatus
{
    FAILURE              = 0b00,
    SUCCESS_FILE_EXISTS  = 0b01,
    SUCCESS_FILE_CREATED = 0b10,
};

#endif // FILE_HANDLING_H
