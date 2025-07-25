#include "argument_handlers.hpp"
#include "arguments/arguments.hpp"
#include "common/labels.hpp"
#include <cstdio>
#include <filesystem>
#include <fstream>

enum class FileStatus
{
    FAILURE              = 0b00,
    SUCCESS_FILE_EXISTS  = 0b01,
    SUCCESS_FILE_CREATED = 0b10,
};

ErrCode helper_WarnIfOptionMissingValue(Option& option, const char* default_value)
{
    if(option.HasValue())
        return Err::SUCCESS;

    if(!flag_Silent)
        printf("%s OptionsHandler - Option [%s, %s] wasn't given a value and will default to %s%s\n", WARN(), option.ShortName(), option.LongName(), default_value, RESET_COLOR());
    return Err::Args::ARGUMENT_OPTION_MISSING;
}

FileStatus CheckFilePath(const char* wish_file_name = nullptr)
{
    if(wish_file_name == nullptr)
        return FileStatus::FAILURE;

    // TODO: remove the std::filesystem code, as the filestreams will fail if the directory doesn't exist, anyways (i think...)
    std::filesystem::path wish_filepath = std::filesystem::absolute(wish_file_name);

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
        if(!suppress_printouts && !flag_Silent)
            printf("%s Cannot write output to '%s'%s\n", WARN(), wish_output_file, RESET_COLOR());
        new_output_file = getenv(constant_ConfigFileLocationEnvironmentVariable);
        if(!new_output_file)
            new_output_file = option_OutputFile.c_str();
        // Call this function again because I want the unique printouts, but I don't want to copy&paste strings
        try_SetOutputFile(new_output_file, suppress_printouts);
        return false;

    case FileStatus::SUCCESS_FILE_EXISTS:
        option_OutputFile = wish_output_file;
        if(!suppress_printouts && !flag_Silent)
            printf("::Output will be appended to %s'%s'%s\n", COLOR(CYAN), wish_output_file, RESET_COLOR());
        return true;

    case FileStatus::SUCCESS_FILE_CREATED:
        option_OutputFile = wish_output_file;
        if(!suppress_printouts && !flag_Silent)
            printf("::Output will be written to %s'%s'%s\n", COLOR(CYAN), wish_output_file, RESET_COLOR());
        return true;
    }
}

ErrCode OptionsHandler(std::vector<Option>* options)
{
    for(Option& option : *options)
    {
        if(option == Options::Output)
        {
            option_Output_WasSpecified = true;

            if(flag_DebugMode)
                printf("::Option '%s' Set to: '%s'\n", option.PrettyName(), option.GetValue());

            if(!option.HasValue())
                return Err::Args::MANDATORY_ARGUMENT_OPTION_MISSING;

            if(!try_SetOutputFile(option.GetValue()))
                try_SetOutputFile(getenv(constant_ConfigFileLocationEnvironmentVariable), true);
        }
    }

    return Err::SUCCESS;
}

ErrCode FlagsHandler(std::vector<Flag>* flags)
{
    for(const Flag& flag : *flags)
    {
        if(flag == Flags::Help)
        {
            printf("%s\n    %s\n", _Help_Printout, _Version_Printout);
            continue;
        }

        if(flag == Flags::Version)
        {
            printf("    %s\n", _Version_Printout);
            continue;
        }

        if(flag == Flags::Verbose)
        {
            flag_Verbose = true;
            printf("Verbosity increased (Nmap output enabled)\n");
            continue;
        }

        if(flag == Flags::Timestamp)
        {
            flag_AddTimestamp = true;
            if((flag_DebugMode || flag_Verbose) && !flag_Silent)
                printf("::Flag Set: %s%s%s\n", COLOR(GREEN), "flag_AddTimestamp", RESET_COLOR());
            continue;
        }

        if(flag == Flags::NoMessage)
        {
            flag_NoMessage = true;
            if((flag_DebugMode || flag_Verbose) && !flag_Silent)
                printf("::Flag Set: %s%s%s\n", COLOR(GREEN), "flag_NoMessage", RESET_COLOR());
            continue;
        }

        if(flag == Flags::NoColor)
        {
            flag_NoColor = true;
            if((flag_DebugMode || flag_Verbose) && !flag_Silent)
                printf("::Flag Set: %s%s%s\n", COLOR(GREEN), "flag_NoColor", RESET_COLOR());
            continue;
        }

        if(flag == Flags::Silent)
        {
            flag_Silent = true;
            if((flag_DebugMode || flag_Verbose) && !flag_Silent)
                printf("::Flag Set: %s%s%s\n", COLOR(GREEN), "flag_Silent", RESET_COLOR());
            continue;
        }

        if(flag == Flags::Minimal)
        {
            flag_Minimal = true;
            if((flag_DebugMode || flag_Verbose) && !flag_Silent)
                printf("::Flag Set: %s%s%s\n", COLOR(GREEN), "flag_Minimal", RESET_COLOR());
            continue;
        }

        if(flag == Flags::YesDNS)
        {
            flag_IncludeDNS = true;
            if((flag_DebugMode || flag_Verbose) && !flag_Silent)
                printf("::Flag Set: %s%s%s\n", COLOR(GREEN), "flag_IncludeDNS", RESET_COLOR());
            continue;
        }

        if(flag == Flags::Pipe)
        {
            flag_NoMessage = true;
            flag_NoColor = true;
            flag_Minimal = true;
            flag_Pipe = true;
            if((flag_DebugMode || flag_Verbose))
                printf("%s Flag Set: %s%s%s\n", DEBUG(), COLOR(GREEN), "flag_Pipe", RESET_COLOR());
        }

        if(flag == Flags::DebugMode)
        {
            flag_DebugMode = true;
            if(!flag_NoMessage && !flag_Silent)
                printf("%s Debug mode enabled!\n", DEBUG());
            continue;
        }

        if(flag == Flags::DebugDry)
        {
            flag_DebugDry = true;
            if(!flag_NoMessage && !flag_Silent)
                printf("%s Nmap will not be run. Instead, the program will attempt to use a pre-existing '.ping' file!\n", DEBUG());
            continue;
        }
    }

    return Err::SUCCESS;
}
