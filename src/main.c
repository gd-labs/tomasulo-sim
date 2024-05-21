#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cli.h"
#include "instruction.h"
#include "station.h"
#include "simulator.h"

#define REG_COUNT 8

int load_program(const char* path, InstList* list)
{
#define MAXBUF 32

    char buf[MAXBUF];
    char* line; 

    FILE *istream = fopen(path, "r");
    if (!istream) {
        return -1;
    }

    while (fgets(buf, sizeof(buf), istream)) {
        line = strtok(buf, "\n");
        instlist_add(list, line);
    }
    return 0;
#undef MAXBUF
}

int main(int argc, char** argv)
{
	Cli cli = {0};
	cli_parse(&cli, argc, argv);

    InstList* program = instlist_new(10);

    load_program(cli.path, program);

    SList* stations = slist_new(10);
    slist_add(stations, "Add1", Adder);
    slist_add(stations, "Add2", Adder);
    slist_add(stations, "Add3", Adder);
    slist_add(stations, "Mul1", Multiplier);
    slist_add(stations, "Mul2", Multiplier);
    slist_add(stations, "Load1", Mem);
    slist_add(stations, "Load2", Mem);

    char* regs[] = {"F0", "F2", "F4", "F6", "F8", "F10", "F12", "F14"};    
    char* reg_contents[] = {"", "", "", "", "", "", "", ""};

    Ctx context = {
		.program = program,
		.stations = stations,
		.issue_width = 1,
		.regfile_size = REG_COUNT,
	};

	char input;
    for (context.cycle = 1; context.cycle < 100; context.cycle++) {
        retire(&context, reg_contents);
        issue(&context, regs, reg_contents);
        execute(&context);
        writeback(&context);

        ctx_show(&context, reg_contents);

        printf("[n]ext, [a]bort => ");
        scanf(" %c", &input);

        switch(input) {
		case 'n' :
			break;
		case 'a':
			return 0;
        }
        system("clear");
    }
}

