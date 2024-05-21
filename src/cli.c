#include <argp.h>

#include "cli.h"

#define USAGE 0x123

PRIVATE const char* doc = "Tomasulo's algorithm simulator";

PRIVATE const char* args_doc = "PATH";

PRIVATE struct argp_option opts[] = {
    {"help", 'h', 0, 0, "Give a detailed help message", 0},
    {"usage", USAGE, 0, 0, "Give a short usage message", 0},
    {"verbose", 'v', 0, false, "Produce verbose output", 0},
    {0},
};

PRIVATE error_t parse_opt(int key, char* arg, struct argp_state* state)
{
    Cli* cli = state->input;

    switch (key) {
    case 'h':
        argp_state_help(state, state->out_stream, ARGP_HELP_STD_HELP);
        break;
    case USAGE:
        argp_state_help(state, state->out_stream, ARGP_HELP_USAGE | ARGP_HELP_EXIT_OK);
        break;
    case 'v':
        cli->verbose = true;
        break;
    case ARGP_KEY_ARG:
        if (state->arg_num > 1) {
            /// Too many arguments.
            argp_usage(state);
        }
        cli->path = arg;
        break;
    case ARGP_KEY_END:
        if (state->arg_num < 1) {
            /// Not enough arguments.
            argp_usage(state);
        }
        break;
    default:
        return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

void cli_parse(Cli* cli, int argc, char** argv)
{
    const struct argp parser = {
        opts,
        parse_opt,
        args_doc,
        doc,
        0,
        0,
        0,
    };

    argp_parse(&parser, argc, argv, ARGP_NO_HELP, 0, cli);
}
