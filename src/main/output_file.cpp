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
    PRINT_DEBUG("'wish_output_file' is '{}'\n", wish_output_file)

    FileStatus file_status = CheckFilePath(wish_output_file);

    if(file_status == FileStatus::FAILURE)
    {
        PRINT_MESSAGE("{} Cannot write output to '{}'{}\n", WARN(), wish_output_file, RESET_COLOR())
        return false;
    }

    Options::Output.SetValue(wish_output_file);

    const char* written_or_appended = (file_status == FileStatus::SUCCESS_FILE_EXISTS) ? "appended" : "written";
    PRINT_OUT("::Output will be {} to {}'{}'{}\n", COLOR(CYAN), written_or_appended, wish_output_file, RESET_COLOR())

    return true;
}

