#include <string.h>
#include <stdio.h>

#include "common.h"
#include "instruction.h"

PRIVATE const char* mnemonics[] = {"ld", "sw", "add", "sub", "mul", "div"};
PRIVATE const int exec_cycles[] = { 1,   1,    2,     2,     4,     8};

PRIVATE void _grow(InstList* list)
{
    Inst* new = realloc(list->data, (list->capacity << 1) * sizeof(Inst));
    if (!new) {
        return;         
    }
    list->capacity <<= 1;
    list->data = new;
}

PRIVATE int _assign_register(int* regid, char* elem)
{
    elem = strtok(NULL, " ,()");
    if (elem[0] == 'F') {
        *regid = atoi(elem + 1);
        return 0;
    } 
	return -8;
}

PRIVATE int _copy_inst_string(Inst* inst, char* text)
{
    inst->text = malloc(strlen(text) + 1);
    if(inst->text) {
        strcpy(inst->text, text);
        return 0;
    } 
	return -9;
}

PRIVATE int _process_loadstore(Inst* inst, char* elem, char* text)
{
    inst->class = Mem;
    if (_assign_register(&inst->rd, elem) != 0) { return -2; }
    if (_copy_inst_string(inst, text) != 0)     { return -3; }

    return 0;
}

PRIVATE int _process_arithmetic(Inst* inst, char* elem, char* text)
{
    if (_assign_register(&inst->rd, elem)  != 0) { return -4; }
    if (_assign_register(&inst->rs1, elem) != 0) { return -5; }
    if (_assign_register(&inst->rs2, elem) != 0) { return -6; }
    if (_copy_inst_string(inst, text) != 0) { return -7; }

    return 0;
}

PRIVATE int _decode(InstList* list, int pos, char* text) { 
    Inst* inst = &list->data[pos];

    char* elem = malloc(strlen(text) + 1);
    if (!elem) { return -1; }
    strcpy(elem, text);

    elem = strtok(elem, " ,()");
	if (!elem) {
		return -10;
	}

	for (int i = 0; i < 6; i++) {
		if (strcmp(elem, mnemonics[i])) {
			continue;
		}
		inst->op = i;
		inst->name = (char *) mnemonics[i];
		inst->remaining = exec_cycles[i];

		switch (i) {
		case Load:
		case Store:
			return _process_loadstore(inst, elem, text);
		case Add:
		case Sub:
			inst->class = Adder;
			return _process_arithmetic(inst, elem, text);
		case Mul:
		case Div:
			inst->class = Multiplier;
			return _process_arithmetic(inst, elem, text);
		}
	}
	return -10;
}

void inst_details(Inst* inst) {
    printf("Text   : %s    name  : %s\n", inst->text, inst->name);
    printf("issue  : %d  execute : %d  writeback : %d  retired : %d  remaining : %d\n",
        inst->issue, inst->execute, inst->writeback, inst->retired, inst->remaining);
    printf("opcode : %d  opclass : %d\n", inst->op, inst->class);
    printf("    rd : %d,     rs1 : %d,       rs2 : %d\n\n", inst->rd, 
        inst->rs1, inst->rs2);
}

void inst_show(Inst* inst) {
    printf("|%20s |%10d |%10d |%10d |%10d |\n",
        inst->text, inst->issue, inst->execute, inst->writeback, inst->retired);
}

InstList* instlist_new(int initial_size) {
    InstList* list = malloc(sizeof(InstList));
    if (!list) {
        return NULL;
    }
    list->capacity = initial_size;
    list->count = 0;

    list->data = malloc(initial_size * sizeof(Inst));
    if (!list->data) {
        return NULL;
    }
	
    return list;
}

int instlist_add(InstList* list, char* text) {
    if (list->count == list->capacity) {
        _grow(list);
    }

    list->data[list->count] = (Inst){0};

    int retval = _decode(list, list->count, text);
    if (!retval) {
        list->count++;
    }
    return retval;
}
