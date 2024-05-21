#ifndef STATION_H
#define STATION_H

#include "instruction.h"

typedef struct {
    OpType type;
    char* name;
    Inst* op;

    bool busy;

    char* vj;
    char* vk;
    char* qj;
    char* qk;
} Station;

typedef struct {
    size_t count;
    size_t capacity;
    Station* data;
} SList;

SList* slist_new(int initial_size);

int slist_add(SList* list, char* namE, OpType type);

void station_show(Station* st);

#endif
