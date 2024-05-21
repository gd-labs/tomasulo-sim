#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <stdlib.h>

#include "common.h"

typedef enum {
    Adder,
    Multiplier,
    Mem,
} OpType;

typedef enum {
    Load,
    Store,
    Add,
    Sub,
    Mul,
    Div,
} OpCode;

typedef struct {
    OpCode op;
    OpType class;
    char* name;

    int rs1;
    int rs2;
    int rd;

    char* text;
    int issue;
    int execute;
    int writeback;
    int retired;
    int remaining;
} Inst;

typedef struct {
    size_t count;
    size_t capacity;
    Inst* data;
} InstList;

InstList* instlist_new(int initial_size);

int instlist_add(InstList* list, char* text);

void inst_show(Inst* inst);

void inst_details(Inst* inst);

#endif
