# castanet v2.0
```
    Usage: castanet [-h|--help] [-v|--version] [-V|--verbose] [-n|--no-output]
                    [--no-color] [--no-message] [-s|--silent] [-m|--minimal]
                    [-p|--pipe] [-o|--output <path>] <number_of_hosts>
    Options:
        -h, --help           print help document
        -v, --version        print program version
        -V, --verbose        make nmap output visible
        -t, --timestamp      output includes a small timestamp at the beginning (overriden by '-m')
        -n, --no-output      do not write output to a file
            --no-color       remove colors from terminal output
            --no-message     suppress only [WARNING], [ERROR], and [DEBUG] messages
        -s, --silent         suppress all terminal printouts (no effect on output file)
        -m, --minimal        only output valid host addresses
        -d, --dns-always     always include DNS names in the output file (mainly used with '-m')
        -p, --pipe           same as '--no-message --no-color --minimal' (designed for piping)
        -o, --output FILE    override the output destination file

    Environment:
        CASTANET_OUTPUT
            Absolute path to the default output file. Unless overwritten by '-n' or '-o', this environment variable
            will always be used as the output location. If the path is invalid, output is redirected to the file
            "castanet_output", in the current working directory.

    Example:
        castanet 14
        castanet -o local_castanet_output 2000
        castanet -p 40 | nmap -sC -sV -Pn -oN huge_scan -iL -

    v2.0.2
```

## What's New in Version Two?
Quite a lot, actually. Castanet is no longer a single script; instead, it's a fully compiled binary written in C++20. It has full argument parsing and handling, a lot more options, an environment variable for setting a different default output file location, and a lot more! Check the help printout for more details, or just use the damn thing yourself! :D
