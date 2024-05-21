#ifndef CLI_H
#define CLI_H

#include "common.h"

typedef struct {
    const char* path;
    bool verbose;
} Cli;

void cli_parse(Cli* cli, int argc, char** argv);

#endif
