#ifndef TOMASULO_H
#define TOMASULO_H

#include "common.h"
#include "instruction.h"
#include "station.h"

typedef struct {
    InstList* program;
    SList*    stations;

    int  cycle;
    int  issue_width;
    int  regfile_size;
    bool complete;
} Ctx;

void issue(Ctx* s, char* reg_names[], char* reg_contents[]);

void execute(Ctx* s);

void writeback(Ctx* s);

void retire(Ctx* s, char* reg_contents[]);

#endif
