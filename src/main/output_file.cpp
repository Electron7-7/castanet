#include "output_file.hpp"

bool try_SetOutputFile(const char* wish_output_file, bool suppress_printouts)
{
    FileStatus file_status = CheckFilePath(wish_output_file);
    const char* new_output_file;

    switch(file_status)
    {
    case FileStatus::FAILURE:
        if(!suppress_printouts && ALLOW_MESSAGE())
            std::print("{} Cannot write output to '{}'{}\n", WARN(), wish_output_file, RESET_COLOR());
        new_output_file = getenv(constant_ConfigFileLocationEnvironmentVariable);
        if(!new_output_file)
            try_SetOutputFile(Options::Output.GetValue(), suppress_printouts);
        return false;

    // TODO: Make these cases better
    case FileStatus::SUCCESS_FILE_EXISTS:
        Options::Output.SetValue(wish_output_file);
        if(!suppress_printouts && ALLOW_PRINTOUT())
            std::print("::Output will be appended to {}'{}'{}\n", COLOR(CYAN), wish_output_file, RESET_COLOR());
        return true;

    case FileStatus::SUCCESS_FILE_CREATED:
        Options::Output.SetValue(wish_output_file);
        if(!suppress_printouts && ALLOW_PRINTOUT())
            std::print("::Output will be written to {}'{}'{}\n", COLOR(CYAN), wish_output_file, RESET_COLOR());
        return true;
    }
}

