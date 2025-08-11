#include "regex_operations.hpp"
#include "common/labels.hpp"
#include "printouts.hpp"

#include <print>
#include <regex>

std::string shitty_DoAllRegexOps(const std::string& input_data)
{
    std::regex header_footer_pattern(R"(\n?#.+\n)");
    std::string output_data = std::regex_replace(input_data, header_footer_pattern, "", std::regex_constants::match_any);

    if(Flags::DebugMode.IsActive())
        std::print("{} Output Data After Removing Header and Footer: \n{}{}{}\n", DEBUG(), COLOR(YELLOW), output_data.c_str(), RESET_COLOR());

    if(Flags::Timestamp.IsActive() && !Flags::Pipe.IsActive())
    {
        std::smatch timestamp_matches;
        std::regex timestamp_pattern(R"(^#.+(\w{3,5} \w+ \d{2} (\d{2}:?){3} \d+))");
        std::regex_search(input_data, timestamp_matches, timestamp_pattern);

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
        std::regex_search(input_data, minimal_matches, ip_and_dns_only_pattern);

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

    return output_data;
}
