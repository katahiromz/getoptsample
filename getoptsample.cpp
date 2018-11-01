// getoptsample.cpp
// Copyright (C) 2018 Katayama Hirofumi MZ <katayama.hirofumi.mz@gmail.com>.
// This file is public domain software.

#include <cstdio>       // standard C I/O
#include <string>       // for std::string
#include <vector>       // for std::vector
#ifdef HAVE_GETOPT_H
    #include <getopt.h> // for getopt_long
#else
    #include "getopt.h" // for getopt_long
#endif

using std::printf;
using std::fprintf;
using std::exit;

// global variables
std::string g_input_file;
std::string g_output_file;
std::vector<std::string> g_non_options;

// return value
enum RET
{
    RET_SUCCESS = 0,
    RET_INVALID_ARGUMENT
};

// show version info
void show_version(void)
{
    printf("getoptsample version 0.0\n");
}

// show help
void show_help(void)
{
    printf("Usage: getoptsample [options]...\n");
    printf("Options:\n");
    printf("--help              Show this help\n");
    printf("--version           Show version info\n");
    printf("-i file\n");
    printf("--input-file=file   An input file\n");
    printf("-o file\n");
    printf("--output-file=file  An output file\n");
}

// option info for getopt_long
struct option opts[] =
{
    { "help", no_argument, NULL, 'h' },
    { "version", no_argument, NULL, 0 },
    { "input-file", required_argument, NULL, 'i' },
    { "output-file", required_argument, NULL, 'o' },
    { NULL, 0, NULL, 0 },
};

// external symbols for getopt_long
extern "C"
{
    extern char* optarg;
    extern int optind, opterr, optopt;
}

// parse the command line
int parse_command_line(int argc, char **argv)
{
    int opt, opt_index;
    std::string arg;

    while ((opt = getopt_long(argc, argv, "hi:o:", opts, &opt_index)) != -1)
    {
        switch (opt)
        {
        case 0:
            arg = opts[opt_index].name;
            if (arg == "version")
            {
                show_version();
                exit(EXIT_SUCCESS);
            }
            break;
        case 'h':
            show_help();
            exit(EXIT_SUCCESS);
            break;
        case 'i':
            g_input_file = optarg;
            break;
        case 'o':
            g_output_file = optarg;
            break;
        case '?':
            /* getopt_long already printed an error message. */
            return RET_INVALID_ARGUMENT;
        default:
            break;
        }
    }

    for (int i = optind; i < argc; ++i)
    {
        g_non_options.push_back(argv[i]);
    }

    return RET_SUCCESS;
}

// do something
int do_something(void)
{
    printf("####################\n");
    printf("### do_something ###\n");
    printf("####################\n");

    printf("input-file: %s\n", g_input_file.c_str());
    printf("output-file: %s\n", g_output_file.c_str());

    printf("non-option arguments:\n");
    for (size_t i = 0; i < g_non_options.size(); ++i)
        printf("g_non_options[%d]: %s\n", (int)i, g_non_options[i].c_str());

    return EXIT_SUCCESS;
}

// the main function
int main(int argc, char **argv)
{
    if (int ret = parse_command_line(argc, argv))
        return EXIT_FAILURE;

    return do_something();
}
