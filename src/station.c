#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "common.h"
#include "station.h"

PRIVATE void _grow(SList* list)
{
    Station* new= realloc(list->data, (list->capacity << 1) * sizeof(Station));
    if (!new) {
        return;         
    }
    list->capacity <<= 1;
    list->data = new;
    return;
}

PRIVATE Station _init_station(char* name, OpType type)
{
    Station s = (Station){0};

    s.name = malloc(sizeof(name));
    if (!s.name) {
        return s;
    }
    strcpy(s.name, name);

    s.type = type;
    return s;
}

SList* slist_new(int initial_size)
{
	SList* list = malloc(sizeof(SList));
    if (!list) {
        return NULL;
    }
    list->capacity = initial_size;
    list->count = 0;

    list->data = malloc(initial_size * sizeof(Station));
    if (!list->data) {
        return NULL;
    }

    return list;
}

int slist_add(SList* list, char* name, OpType type)
{
    if (list->count == list->capacity) {
        _grow(list);
    }

    if (list->count == list->capacity) {
        // _grow failed, aborting
        return -1;
    }

    list->data[list->count] = _init_station(name, type);
    list->count++;
    return 0;
}

void station_show(Station* st)
{
	char* busy = (st->busy == true) ? "yes" : "no";
	char* op = (st->busy == true) ? st->op->name : "";
	char* vj = (st->vj != NULL) ? st->vj : "";
	char* vk = (st->vk != NULL) ? st->vk : "";
	char* qj = (st->qj != NULL) ? st->qj : "";
	char* qk = (st->qk != NULL) ? st->qk : "";

	printf("|%9s |%7s |%8s |%8s |%8s |%8s |%8s |\n", 
			st->name, busy, op, vj, vk, qj, qk);
}
