#include "arguments.hpp"
#include "printouts.hpp"
#include "file_handling.hpp"
#include "common/labels.hpp"
#include "getargs/argument_parser.hpp"

#include <print>
#include <string>
#include <fstream>
#include <filesystem>
#include <unistd.h>
#include <regex>

#define NMAP "/usr/bin/nmap"
#define NMAP_PORTS "21-25,42,53,80,135-139,443-445,890,1337,1433-1434,1512,3398,5555,8080,9090"
#define NMAP_ARGS " -T5 -Pn -R -p" NMAP_PORTS " --open "
#define NMAP_OUT_FLAG " -oN "
#define NMAP_TEMP_OUT ".ping"
#define NMAP_HOSTS " -iR "
#define NMAP_BG " &>/dev/null"

std::string constant_ConfigFileLocationEnvironmentVariable = "CASTANET_OUTPUT";
std::string constant_DefaultOutputFile = "castanet_output";

int main(int argc, char** argv)
{
    // Add valid flags
    ArgumentParser::AddFlag(&Flags::Help);
    ArgumentParser::AddFlag(&Flags::Version);
    ArgumentParser::AddFlag(&Flags::Verbose);
    ArgumentParser::AddFlag(&Flags::Timestamp);
    ArgumentParser::AddFlag(&Flags::DryRun);
    ArgumentParser::AddFlag(&Flags::NoColor);
    ArgumentParser::AddFlag(&Flags::NoMessage);
    ArgumentParser::AddFlag(&Flags::Silent);
    ArgumentParser::AddFlag(&Flags::Minimal);
    ArgumentParser::AddFlag(&Flags::YesDNS);
    ArgumentParser::AddFlag(&Flags::Pipe);
    ArgumentParser::AddFlag(&Flags::DebugMode);
    ArgumentParser::AddFlag(&Flags::DebugDry);
    ArgumentParser::AddFlag(&Flags::DebugAll);

    // Add valid options
    ArgumentParser::AddOption(&Options::Output);

    // Parse all arguments
    int last_argc_processed = ArgumentParser::ParseArguments(argc, argv);

    if(last_argc_processed == ARG_STATUS_FAILED || last_argc_processed >= argc || Flags::Help.IsActive())
    {
        std::print("{}\n    {}\n", _Help_Printout, _Version_Printout);
        return (Flags::Help.IsActive()) ? 0 : 1; // '--help' should return '0', no final argument should return '1'
    }

    if(Flags::Pipe.IsActive())
    {
        Flags::DryRun.Activate();
        Flags::NoColor.Activate();
        Flags::Minimal.Activate();
    }

    bool use_environment_variable = true;

    if(Options::Output.IsActive())
        use_environment_variable = try_SetOutputFile(Options::Output.GetValue());

    if(use_environment_variable)
        try_SetOutputFile(getenv(constant_ConfigFileLocationEnvironmentVariable.c_str()), true);
    else
        try_SetOutputFile(constant_DefaultOutputFile.c_str());

    long argument_NumberOfHosts = 0;

    try
    {
        argument_NumberOfHosts = std::stoll(argv[last_argc_processed]);
    }
    catch(std::invalid_argument const& exception)
    {
        PRINT_MESSAGE("{} Invalid number of hosts: '{}'{}\n", ERROR(), argv[last_argc_processed], RESET_COLOR())
        return 1;
    }

    if(access("/usr/bin/nmap", X_OK) != 0)
    {
        PRINT_MESSAGE("{} Nmap not installed! Please install nmap{}\n", ERROR(), RESET_COLOR())
        return 1;
    }

    std::string nmap_command_line = (NMAP NMAP_ARGS NMAP_HOSTS) + std::to_string(argument_NumberOfHosts) + (NMAP_OUT_FLAG NMAP_TEMP_OUT);

    if(!Flags::Verbose.IsActive())
        nmap_command_line += NMAP_BG;

    if(Flags::Verbose.IsActive())
        PRINT_OUT("::Nmap Command: {}\n", nmap_command_line.c_str());

    int nmap_success = 0;

    PRINTOUT("::Running Nmap\n")

    if(!Flags::DebugDry.IsActive() && !Flags::DebugAll.IsActive())
        nmap_success = system(nmap_command_line.c_str());

    if(Flags::DebugAll.IsActive())
        std::print("{} Now is when nmap would have run\n", DEBUG());

    std::ifstream nmap_temp_file(NMAP_TEMP_OUT);

    if(!Flags::DebugAll.IsActive() && nmap_success != 0 && !nmap_temp_file)
    {
        std::print("{} Unable to read nmap output! Nmap exit code: %d{}\n", ERROR(), nmap_success, RESET_COLOR());
        nmap_temp_file.close();
        return 1;
    }

    PRINTOUT("::Nmap finished\n")

    std::string nmap_output;
    std::stringstream nmap_output_buffer;

    if(!Flags::DebugAll.IsActive())
        nmap_output_buffer << nmap_temp_file.rdbuf();
    else
        std::print("{} Now is when 'nmap_output' would have been buffered with the data inside '.ping'\n", DEBUG());

    nmap_temp_file.close();

    if(!Flags::DebugAll.IsActive() && Flags::DebugMode.IsActive())
        std::print("{} Nmap output file '{}' will not be deleted\n", DEBUG(), NMAP_TEMP_OUT);
    else
        std::filesystem::remove(std::filesystem::path(NMAP_TEMP_OUT));

    nmap_output = nmap_output_buffer.str();
    nmap_output_buffer.clear();

    PRINTOUT("::Parsing output\n")

    if(Flags::DebugAll.IsActive())
    {
        std::print("{} This is when all regex operations would have run. Instead, the program will now early return\n", DEBUG());
        return 0;
    }

    if(Flags::DebugMode.IsActive())
        std::print("{} Nmap Output:\n{}{}{}\n", DEBUG(), COLOR(YELLOW), nmap_output.c_str(), RESET_COLOR());

    std::regex header_footer_pattern(R"(\n?#.+\n)");
    std::string output_data = std::regex_replace(nmap_output, header_footer_pattern, "", std::regex_constants::match_any);

    if(Flags::DebugMode.IsActive())
        std::print("{} Output Data After Removing Header and Footer: \n{}{}{}\n", DEBUG(), COLOR(YELLOW), output_data.c_str(), RESET_COLOR());

    if(Flags::Timestamp.IsActive() && !Flags::Pipe.IsActive())
    {
        std::smatch timestamp_matches;
        std::regex timestamp_pattern(R"(^#.+(\w{3,5} \w+ \d{2} (\d{2}:?){3} \d+))");
        std::regex_search(nmap_output, timestamp_matches, timestamp_pattern);

        output_data.insert(0, "[Timestamp: " + std::string(timestamp_matches.str(1)) + "]\n");

        if(Flags::DebugMode.IsActive())
            std::print("{} Output Data After Adding Timestamp: \n{}{}{}\n", DEBUG(), COLOR(YELLOW), output_data.c_str(), RESET_COLOR());
    }

    if(Flags::Minimal.IsActive())
    {
        if(Flags::Timestamp.IsActive())
            PRINT_MESSAGE("{} A timestamp will be added to the output;  if you want to pipe output to another program, use the pipe flag [-p|--pipe]){}\n", WARN(), RESET_COLOR())

        if(Flags::YesDNS.IsActive())
            PRINT_MESSAGE("{} DNS names will be added to the output (as well as newlines); if you want to pipe output to another program, use the pipe flag [-p|--pipe]'{}\n", WARN(), RESET_COLOR())

        std::string output_data_copy = output_data;
        output_data = "";

        std::smatch minimal_matches;
        std::regex ip_and_dns_only_pattern(R"(Nmap scan report for (?:((?:\w+(?:\.|-)?)+) )?(?:\(?((?:\d+\.?)+)\)?))");
        std::regex_search(nmap_output, minimal_matches, ip_and_dns_only_pattern);

        auto begin = std::sregex_iterator(output_data_copy.begin(), output_data_copy.end(), ip_and_dns_only_pattern);
        auto end = std::sregex_iterator();

        for(std::sregex_iterator i = begin; i != end; ++i)
        {
            std::smatch match = *i;

            if(Flags::YesDNS.IsActive() && !Flags::Pipe.IsActive())
            {
                if(!match.str(1).empty())
                    output_data += match.str(1) + " ";

                output_data += match.str(2) + "\n";
                continue;
            }

            if(Flags::DebugMode.IsActive())
                std::print("{} Matches: {}Capture Group 0: '{}' Capture Group 1: '{}' Capture Group 2: '{}'{}\n", DEBUG(), COLOR(GREEN), match.str().c_str(), match.str(1).c_str(), match.str(2).c_str(), RESET_COLOR());

            std::string ip_address = match.str(2);
            output_data += ip_address + "\n";
        }

        output_data += "\n";

        if(Flags::DebugMode.IsActive())
            std::print("{} Output Data After Minimizing: \n{}{}{}\n", DEBUG(), COLOR(YELLOW), output_data.c_str(), RESET_COLOR());
    }

    else
        output_data += "\n";

    std::ofstream castanet_output_file(Options::Output.GetValue(), std::ios::app);

    if(output_data.empty())
    {
        PRINTOUT("::No valid hosts found!\n");

        castanet_output_file.close(); // FIXME: is this allowed?
        return 0;
    }

    if(!castanet_output_file)
    {
        PRINT_MESSAGE("{} Output file '{}' is unable to be opened/written to! The program will print nmap's captured output before aborting.\n{}Nmap Output:{}\n{}\n", ERROR(), Options::Output.GetValue(), COLOR_BOLD(GREEN), RESET_COLOR(), nmap_output);

        castanet_output_file.close(); // FIXME: is this allowed?
        return 1;
    }

    if(Flags::DryRun.IsActive())
        std::print("{}\n", output_data);
    else
    {
        castanet_output_file << output_data;
        castanet_output_file.close();

        PRINTOUT("::Castanet Finished\n")
    }

    return 0;
}

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
