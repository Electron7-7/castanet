#include "output_file.hpp"
#include "arguments.hpp"
#include "printouts.hpp"
#include "common/labels.hpp"
#include "getargs/argument.hpp"

#include <string>
#include <filesystem>
#include <fstream>
#include <print>

FileStatus CheckFilePath(const std::string& wish_file_name = "")
{
    if(wish_file_name.empty())
        return FileStatus::FAILURE;

    // TODO: remove the std::filesystem code, as the filestreams will fail if the directory doesn't exist, anyways (i think...)
    std::filesystem::path wish_filepath = std::filesystem::absolute(std::filesystem::path(wish_file_name));

    if(exists(wish_filepath.remove_filename()))
    {
        std::ifstream file_already_exists(wish_filepath.string());
        if(file_already_exists)
        {
            file_already_exists.close();
            return FileStatus::SUCCESS_FILE_EXISTS;
        }

        std::ofstream can_write_to_file(wish_filepath.string());
        if(can_write_to_file)
        {
            can_write_to_file.close();
            return FileStatus::SUCCESS_FILE_CREATED;
        }
    }

    return FileStatus::FAILURE;
}


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

