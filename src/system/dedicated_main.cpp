#include "arguments/arguments.hpp"
#include "arguments/arguments_parser.hpp"

int main(int argc, char** argv)
{
    // Add valid flags
    global_ArgumentsParser->AddFlag(Flags::Help);
    global_ArgumentsParser->AddFlag(Flags::Version);
    global_ArgumentsParser->AddFlag(Flags::NoColor);
    global_ArgumentsParser->AddFlag(Flags::DebugMode);

    // Add valid options
    global_ArgumentsParser->AddOption(Options::Output);
    global_ArgumentsParser->AddOption(Options::SetConfig);

    // Parse all arguments
    global_ArgumentsParser->ParseArguments(argc, argv);

    // Handle flags

    // Handle options

    return 0;
}
