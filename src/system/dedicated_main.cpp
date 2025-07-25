#include "arguments/arguments.hpp"
#include "arguments/arguments_parser.hpp"
#include "argument_handlers.hpp"
#include "argument_variables.hpp"
#include "common/labels.hpp"
#include <filesystem>
#include <regex>
#include <unistd.h>
#include <fstream>

#define NMAP "/usr/bin/nmap"
#define NMAP_PORTS "21-25,42,53,80,135-139,443-445,890,1337,1433-1434,1512,3398,5555,8080,9090"
#define NMAP_ARGS " -T5 -Pn -R -p" NMAP_PORTS " --open "
#define NMAP_OUT_FLAG " -oN "
#define NMAP_TEMP_OUT ".ping"
#define NMAP_HOSTS " -iR "
#define NMAP_BG " &>/dev/null"

int main(int argc, char** argv)
{
    // FIXME: Copied code
    if(argc <= 1)
    {
        printf("%s\n    %s\n", _Help_Printout, _Version_Printout);
        return 1;
    }

    // Add valid flags
    global_ArgumentsParser->AddFlag(Flags::Help);
    global_ArgumentsParser->AddFlag(Flags::Version);
    global_ArgumentsParser->AddFlag(Flags::Verbose);
    global_ArgumentsParser->AddFlag(Flags::Timestamp);
    global_ArgumentsParser->AddFlag(Flags::NoColor);
    global_ArgumentsParser->AddFlag(Flags::NoMessage);
    global_ArgumentsParser->AddFlag(Flags::Silent);
    global_ArgumentsParser->AddFlag(Flags::Minimal);
    global_ArgumentsParser->AddFlag(Flags::YesDNS);
    global_ArgumentsParser->AddFlag(Flags::Pipe);
    global_ArgumentsParser->AddFlag(Flags::DebugMode);
    global_ArgumentsParser->AddFlag(Flags::DebugDry);

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

    // FIXME: Copied code
    if(++last_argc_processed >= argc)
    {
        printf("%s\n    %s\n", _Help_Printout, _Version_Printout);
        return 1;
    }

    try
    {
        argument_NumberOfHosts = std::stoll(argv[last_argc_processed]);
    }
    catch(std::invalid_argument const& exception)
    {
        if(!flag_Silent && !flag_NoMessage)
            printf("%s Invalid number of hosts: '%s'%s\n", ERROR(), argv[last_argc_processed], RESET_COLOR());
        return 1;
    }

    // FIXME: Copied code
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

    const char* output_file_var = getenv(constant_ConfigFileLocationEnvironmentVariable);

    if(output_file_var != nullptr && !option_Output_WasSpecified)
        try_SetOutputFile(output_file_var);

    std::string nmap_command_line = (NMAP NMAP_ARGS NMAP_HOSTS) + std::to_string(argument_NumberOfHosts) + (NMAP_OUT_FLAG NMAP_TEMP_OUT);

    if(!flag_Verbose)
        nmap_command_line += NMAP_BG;

    if(flag_Verbose)
        printf("::Nmap Command: %s\n", nmap_command_line.c_str());

    int nmap_success = 0;

    if(!flag_DebugDry)
        nmap_success = system(nmap_command_line.c_str());

    std::ifstream nmap_temp_file(NMAP_TEMP_OUT);

    if(nmap_success != 0 && !nmap_temp_file)
    {
        if(!flag_Silent && !flag_NoMessage)
            printf("%s Unable to read nmap output! Nmap exit code: %d%s\n", ERROR(), nmap_success, RESET_COLOR());
        nmap_temp_file.close();
        return 1;
    }

    std::string nmap_output;
    std::stringstream nmap_output_buffer;

    nmap_output_buffer << nmap_temp_file.rdbuf();
    nmap_temp_file.close();

    if(flag_DebugMode)
        printf("%s Nmap output file '%s' will not be deleted\n", DEBUG(), NMAP_TEMP_OUT);
    else
        std::filesystem::remove(std::filesystem::path(NMAP_TEMP_OUT));

    nmap_output = nmap_output_buffer.str();
    nmap_output_buffer.clear();

    if(flag_DebugMode)
        printf("%s Nmap Output:\n%s%s%s\n", DEBUG(), COLOR(YELLOW), nmap_output.c_str(), RESET_COLOR());

    std::regex header_footer_pattern(R"(\n?#.+\n)");
    std::string output_data = std::regex_replace(nmap_output, header_footer_pattern, "", std::regex_constants::match_any);

    if(flag_DebugMode)
        printf("%s Output Data After Removing Header and Footer: \n%s%s%s\n", DEBUG(), COLOR(YELLOW), output_data.c_str(), RESET_COLOR());

    if(flag_AddTimestamp && !flag_Pipe)
    {
        std::smatch timestamp_matches;
        std::regex timestamp_pattern(R"(^#.+(\w{3,5} \w+ \d{2} (\d{2}:?){3} \d+))");
        std::regex_search(nmap_output, timestamp_matches, timestamp_pattern);

        output_data.insert(0, "[Timestamp: " + std::string(timestamp_matches.str(1)) + "]\n");

        if(flag_DebugMode)
            printf("%s Output Data After Adding Timestamp: \n%s%s%s\n", DEBUG(), COLOR(YELLOW), output_data.c_str(), RESET_COLOR());
    }

    if(flag_Minimal)
    {
        if(flag_AddTimestamp && (!flag_NoMessage && !flag_Silent))
            printf("%s A timestamp will be added to the output;  if you want to pipe output to another program, use the pipe flag [-p|--pipe])%s\n", WARN(), RESET_COLOR());

        if(flag_IncludeDNS && (!flag_NoMessage && !flag_Silent))
            printf("%s DNS names will be added to the output (as well as newlines); if you want to pipe output to another program, use the pipe flag [-p|--pipe]'%s\n", WARN(), RESET_COLOR());

        std::string output_data_copy = output_data;
        output_data = "";

        std::smatch minimal_matches;
        std::regex ip_and_dns_only_pattern(R"(Nmap scan report for (?:((?:\w\.?)+)\s\(((?:\d{1,3}\.?){4})\)|((?:\d{1,3}\.?)+)))");
        std::regex_search(nmap_output, minimal_matches, ip_and_dns_only_pattern);

        auto begin = std::sregex_iterator(output_data_copy.begin(), output_data_copy.end(), ip_and_dns_only_pattern);
        auto end = std::sregex_iterator();

        for(std::sregex_iterator i = begin; i != end; ++i)
        {
            std::smatch match = *i;

            if(flag_IncludeDNS && !flag_Pipe)
            {
                int ip_match_number = 3;

                if(!match.str(1).empty() && !match.str(2).empty())
                {
                    output_data += match.str(1) + " ";
                    ip_match_number = 2;
                }

                output_data += "(" + match.str(ip_match_number) + ")\n";
                continue;
            }

            std::string ip_address = (match.str(2).empty()) ? match.str(3) : match.str(2);
            output_data += ip_address + "\n";

            if(flag_DebugMode)
                printf("%s Match: %s'%s', '%s', '%s', '%s'%s\n", DEBUG(), COLOR(GREEN), match.str().c_str(), match.str(1).c_str(), match.str(2).c_str(), match.str(3).c_str(), RESET_COLOR());
        }

        output_data += "\n";

        if(flag_DebugMode)
            printf("%s Output Data After Minimizing: \n%s%s%s\n", DEBUG(), COLOR(YELLOW), output_data.c_str(), RESET_COLOR());
    }

    else
        output_data += "\n";

    std::ofstream castanet_output_file(option_OutputFile, std::ios::app);

    if(output_data.empty())
    {
        if(!flag_NoMessage && !flag_Silent)
            printf("::No valid hosts found!\n");

        castanet_output_file.close(); // FIXME: is this allowed?
        return 0;
    }

    if(!castanet_output_file)
    {
        if(!flag_Silent)
        {
            printf("%s Output file '%s' is unable to be opened/written to! The program will print nmap's captured output before aborting.%s\n", ERROR(), option_OutputFile.c_str(), RESET_COLOR());
            printf("\n%sNmap Output:%s\n%s\n", COLOR_BOLD(GREEN), RESET_COLOR(), nmap_output.c_str());
        }

        castanet_output_file.close(); // FIXME: is this allowed?
        return 1;
    }

    castanet_output_file << output_data;
    castanet_output_file.close();

    if(!flag_Silent)
        printf("::Castanet Finished\n");

    return 0;
}
