#include "arguments/arguments.hpp"
#include "arguments/arguments_parser.hpp"
#include "argument_handlers.hpp"
#include "argument_variables.hpp"
#include "common/labels.hpp"
#include <unistd.h>
#include <fstream>

#define NMAP "/usr/bin/nmap"
#define NMAP_OUT
#define NMAP_ARGS " -T5 -Pn -R -p" NMAP_PORTS " --open -oN " NMAP_OUT
#define NMAP_HOSTS " -iR "
#define NMAP_BG " &>/dev/null"
#define NMAP_PORTS "21-25,42,53,80,135-139,443-445,890,1337,1433-1434,1512,3398,5555,8080,9090"

int main(int argc, char** argv)
{
    // Add valid flags
    global_ArgumentsParser->AddFlag(Flags::Help);
    global_ArgumentsParser->AddFlag(Flags::Version);
    global_ArgumentsParser->AddFlag(Flags::NoColor);
    global_ArgumentsParser->AddFlag(Flags::NoMessage);
    global_ArgumentsParser->AddFlag(Flags::Silent);
    global_ArgumentsParser->AddFlag(Flags::Pipe);
    global_ArgumentsParser->AddFlag(Flags::DebugMode);

    // Add valid options
    global_ArgumentsParser->AddOption(Options::Output);

    // Parse all arguments
    global_ArgumentsParser->ParseArguments(argc, argv);

    // Handle flags
    if(unsigned short return_value = FlagsHandler(global_ArgumentsParser->GetFlags()) != Err::SUCCESS)
        return return_value;

    // Handle options
    if(unsigned short return_value = OptionsHandler(global_ArgumentsParser->GetOptions()) != Err::SUCCESS)
        return return_value;

    if(argument_NumberOfHosts <= 0)
    {
        printf("%s\n    %s\n", _Help_Printout, _Version_Printout);
        return 1;
    }

    if(access("/usr/bin/nmap", X_OK) != 0)
    {
        if(!flag_Silent && !flag_NoMessage)
            printf("%s Nmap not installed! Please install nmap%s\n", ERROR(), RESET_COLOR());
        return 1;
    }

    std::string nmap_command_line = NMAP NMAP_ARGS NMAP_HOSTS + std::to_string(argument_NumberOfHosts) + NMAP_BG;

    int nmap_success = system(nmap_command_line.c_str());
    std::ifstream nmap_output(NMAP_HOSTS);

    if(!nmap_success && !nmap_output)
    {
        if(!flag_Silent && !flag_NoMessage)
            printf("%s Unable to read nmap output! Nmap exit code: %d%s\n", ERROR(), nmap_success, RESET_COLOR());
        nmap_output.close();
        return 1;
    }



    return 0;
}
