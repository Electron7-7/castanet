#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include "argument.hpp"

namespace Flags
{
    const Flag Help      ("--help", "-h");
    const Flag Version   ("--version", "-v");
    const Flag NoColor   ("--no-color");
    const Flag NoMessage ("--no-message");
    const Flag Silent    ("--silent", "-s");
    const Flag Minimal   ("--minimal", "-m");
    const Flag Pipe      ("--pipe", "-p");
    const Flag DebugMode ("--debug");
}

namespace Options
{
    const Option Output    ("--output", "-o", true);
}

constexpr const char* _Help_Printout =
R"~(    Usage: castanet [-h|--help] [-v|--version] [-n|--no-output] [--no-color]
                    [--no-message] [-s|--silent] [-m|--minimal] [-p|--pipe]
                    [-o|--output <path>] <number_of_hosts>
    Options:
        -h, --help           print help document
        -v, --version        print program version
        -n, --no-output      do not write output to a file
            --no-color       remove colors from terminal output
            --no-message     suppress all [WARNING], [ERROR], and [DEBUG] messages
        -s, --silent         suppress all terminal printouts (no effect on output file)
        -m, --minimal        only output valid host addresses
        -p, --pipe           same as '--no-message --no-color --minimal' (designed for piping)
        -o, --output FILE    override the output destination file

    Environment:
        CASTANET_OUTPUT
            Absolute path to the default output file. Unless overwritten by '-n' or '-o', this environment variable
            will always be used as the output location. If the path is invalid, output is redirected to 'castanet_output'
            in the current working directory.

    Example:
        castanet 14
        castanet -o local_castanet_output 2000
        castanet -p 40 | nmap -sC -sV -Pn -oN huge_scan -iL -
)~";

constexpr const char* _Version_Printout = "castanet v2.0.0";

#endif // ARGUMENTS_H
