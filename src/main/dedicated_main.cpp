#include "printouts.hpp"
#include "arguments.hpp"
#include "regex_operations.hpp"
#include "output_file.hpp"
#include "common/labels.hpp"
#include "getargs/argument_parser.hpp"

#include <print>
#include <string>
#include <fstream>
#include <filesystem>
#include <unistd.h>

#define NMAP "/usr/bin/nmap"
#define NMAP_PORTS "21-25,42,53,80,135-139,443-445,890,1337,1433-1434,1512,3398,5555,8080,9090"
#define NMAP_ARGS " -T5 -Pn -R -p" NMAP_PORTS " --open "
#define NMAP_OUT_FLAG " -oN "
#define NMAP_TEMP_OUT ".ping"
#define NMAP_HOSTS " -iR "
#define NMAP_BG " &>/dev/null"

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
        Flags::Silent.Activate();
        Flags::DryRun.Activate();
        Flags::NoColor.Activate();
        Flags::Minimal.Activate();
    }

    if(Flags::DebugAll.IsActive())
    {
        Flags::DebugMode.Activate();
        Flags::DebugDry.Activate();
    }

    if(Flags::DebugMode.IsActive())
        Flags::Verbose.Activate();

    std::string try_output_location = Options::Output.GetValue();

    if(!Options::Output.HasValue())
    {
        PRINTDEBUG("Choosing the default output file location\n")
        PRINT_DEBUG("Starting by trying the environment variable '{}'\n", constant_ConfigFileLocationEnvironmentVariable)
        const char* try_environment_variable = getenv(constant_ConfigFileLocationEnvironmentVariable);

        if(try_environment_variable != NULL)
        { try_output_location = try_environment_variable; }
        else
        { PRINT_DEBUG("Environment variable '{}' is unset. Using the default output location '{}'\n", constant_ConfigFileLocationEnvironmentVariable, constant_DefaultOutputFile) }
    }

    if(Flags::Verbose.IsActive())
    { PRINT_OUT("::Validating output file location: {}'{}'{}\n", COLOR_BOLD(CYAN), try_output_location, COLOR_RESET) }

    try_SetOutputFile(try_output_location.c_str());

    long argument_NumberOfHosts = 0;

    try
    {
        argument_NumberOfHosts = std::stoll(argv[++last_argc_processed]);
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

    std::string nmap_command_line = \
    (NMAP NMAP_ARGS NMAP_HOSTS) + std::to_string(argument_NumberOfHosts) + (NMAP_OUT_FLAG NMAP_TEMP_OUT);

    if(Flags::Verbose.IsActive())
    { PRINTOUT("::Running Nmap\n") }
    else
    {
        nmap_command_line += NMAP_BG;
        PRINT_OUT("::Running '{}'\n", nmap_command_line.c_str())
    }

    int nmap_status = 1;

    if(Flags::DebugDry.IsActive())
    { PRINT_DEBUG("{} Now is when nmap would have run\n", DEBUG()) }
    else
    { nmap_status = system(nmap_command_line.c_str()); }

    std::ifstream nmap_temp_file(NMAP_TEMP_OUT);

    if(!Flags::DebugDry.IsActive() && (nmap_status != 0 || !nmap_temp_file))
    {
        PRINT_MESSAGE("{} Unable to read nmap output! Nmap exit code: {}\n", ERROR(), nmap_status, RESET_COLOR())
        nmap_temp_file.close();
        return nmap_status;
    }

    if(!Flags::Verbose.IsActive())
    { PRINTOUT("::Nmap finished\n") }

    std::string nmap_output;
    std::stringstream nmap_output_buffer;

    if(Flags::DebugDry.IsActive())
    { PRINT_DEBUG("{} Now is when 'nmap_output' would have been buffered with the data inside '.ping'\n", DEBUG()) }
    else
    { nmap_output_buffer << nmap_temp_file.rdbuf(); }

    nmap_temp_file.close();

    if(Flags::DebugDry.IsActive())
    { PRINT_DEBUG("{} Nmap output file '{}' will not be deleted\n", DEBUG(), NMAP_TEMP_OUT) }
    else
    { std::filesystem::remove(std::filesystem::path(NMAP_TEMP_OUT)); }

    nmap_output = nmap_output_buffer.str();
    nmap_output_buffer.clear();

    if(Flags::Verbose.IsActive())
    { PRINT_OUT("::Writing Nmap Output to '{}'\n", Options::Output.GetValue()) }

    if(!Flags::DebugDry.IsActive())
    { PRINT_DEBUG("Nmap Output:\n{}{}{}\n", COLOR(YELLOW), nmap_output.c_str(), RESET_COLOR()) }
    else
    {
        PRINT_DEBUG("{} This is when all regex operations would have run. Instead, the program will now early return\n", DEBUG())
        return 0;
    }

    std::string output_data = shitty_DoAllRegexOps(nmap_output);

    PRINTOUT("::Parsing output\n")

    std::ofstream castanet_output_file(Options::Output.GetValue(), std::ios::app);

    if(output_data.empty())
    {
        PRINTOUT("::No valid hosts were found, so no output will be written\n")
        castanet_output_file.close(); // FIXME: is this allowed?
        return 0;
    }

    if(!castanet_output_file)
    {
        PRINT_MESSAGE("{} Output file '{}' is unable to be opened/written to! The program will print nmap's captured output before aborting.\n{}Nmap Output:{}\n{}\n", ERROR(), Options::Output.GetValue(), COLOR_BOLD(GREEN), RESET_COLOR(), nmap_output)
        castanet_output_file.close(); // FIXME: is this allowed?
        return 1;
    }

    if(Flags::DryRun.IsActive())
    { PRINT_DEBUG("{}\n", output_data) }
    else
    {
        castanet_output_file << output_data;
        castanet_output_file.close();

        PRINTOUT("::Castanet Finished\n")
    }

    return 0;
}
