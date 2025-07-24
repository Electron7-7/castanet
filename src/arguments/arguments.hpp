#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include "argument.hpp"

namespace Flags
{
    const Flag Help      ("--help", "-h");
    const Flag Version   ("--version", "-v");
    const Flag NoColor   ("--no-color");
    const Flag DebugMode ("--debug");
}

namespace Options
{
    const Option Output    ("--output", "-o", true);
    const Option SetConfig ("--set-config", "", true);
}

constexpr const char* _Help_Printout =
R"~(    Usage: makemake [-hv] [-o|--output <output_file>] [--default-output <output_file>] [<number_of_hosts>]
        -h, --help              print help document
        -v, --version           print program version
        --no-color              remove colors from terminal output
        -o, --output OUTPUT     set the output file to 'OUTPUT'
        --set-config CONFIG     set the config file location to 'CONFIG'

    Example:
        castanet 100
)~";

constexpr const char* _Version_Printout = "castanet v2.0.0";

#endif // ARGUMENTS_H
