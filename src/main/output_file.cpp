#include "output_file.hpp"
#include "arguments.hpp"
#include "printouts.hpp"
#include "common/labels.hpp"
#include "getargs/argument.hpp"

#include <string>
#include <filesystem>
#include <fstream>

FileStatus CheckFilePath(const std::string& wish_file_name)
{
    if(wish_file_name.empty())
        return FileStatus::FAILURE;

    std::filesystem::path wish_filepath  = std::filesystem::absolute(std::filesystem::path(wish_file_name));
    std::filesystem::path wish_directory = std::filesystem::path(wish_filepath).remove_filename();

    if(!exists(wish_directory))
    {
        PRINT_MESSAGE("{} Directory '{}' does not exist{}\n", WARN(), wish_filepath.string(), COLOR_RESET)
        return FileStatus::FAILURE;
    }

    std::ifstream file_already_exists(wish_filepath);
    if(file_already_exists)
    {
        if(Flags::Verbose.IsActive())
            PRINT_OUT("::File '{}' is accessible\n", wish_filepath.generic_string())
        file_already_exists.close();
        return FileStatus::SUCCESS_FILE_EXISTS;
    }

    std::ofstream can_write_to_file(wish_filepath);
    if(can_write_to_file)
    {
        if(Flags::Verbose.IsActive())
            PRINT_OUT("::File '{}' can be created\n", wish_filepath.string())
        can_write_to_file.close();
        return FileStatus::SUCCESS_FILE_CREATED;
    }

    if(Flags::Verbose.IsActive())
    {
        PRINT_MESSAGE("{} Failed to access/create file at '{}'{}\n", WARN(), wish_filepath.string(), COLOR_RESET)
        PRINTOUT("\t::This is usually due to insufficient permissions. Make sure you know what you're doing before you use sudo...")
    }
    return FileStatus::FAILURE;
}

bool try_SetOutputFile(const char* wish_output_file)
{
    // FIXME: This 'NULL' check never happens, now, because I do that check in the main function

    if(wish_output_file == NULL)
    {
        PRINT_DEBUG("'wish_output_file' is {}NULL{}; this is due to the environment variable '{}' not being set\n", COLOR_BOLD(RED), constant_ConfigFileLocationEnvironmentVariable, COLOR_RESET)
        PRINT_DEBUG("\t- This isn't a big deal, btw, it just means that the program will use '{}' for the default location instead\n", constant_DefaultOutputFile)
        return false;
    }

    PRINT_DEBUG("'wish_output_file' is '{}'\n", wish_output_file)

    FileStatus file_status = CheckFilePath(wish_output_file);
    const char* new_output_file;

    switch(file_status)
    {
    case FileStatus::FAILURE:
        PRINT_MESSAGE("{} Cannot write output to '{}'{}\n", WARN(), wish_output_file, RESET_COLOR())
        return false;

    // TODO: Make these cases better
    case FileStatus::SUCCESS_FILE_EXISTS:
        PRINT_OUT("::Output will be appended to {}'{}'{}\n", COLOR(CYAN), wish_output_file, RESET_COLOR())
        break;

    case FileStatus::SUCCESS_FILE_CREATED:
        PRINT_OUT("::Output will be written to {}'{}'{}\n", COLOR(CYAN), wish_output_file, RESET_COLOR())
        break;
    }

    Options::Output.SetValue(wish_output_file);
    return true;
}

