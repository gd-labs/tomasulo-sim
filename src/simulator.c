#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "simulator.h"
#include "instruction.h"
#include "station.h"

PRIVATE Station* _find_station(Inst* inst, SList* rs)
{
    for(size_t i = 0; i < rs->count; i++) {
        if(rs->data[i].type == inst->class) {
            if(!rs->data[i].busy) {
                return &rs->data[i];
            }
        }
    }
    return NULL;
}

PRIVATE void _fill_station(Station* st, Inst* inst, 
            				char* reg_names[], char* reg_contents[])
{
    st->busy = true;
    st->op = inst;
    reg_contents[inst->rd >>1] = st->name;

    if (inst->class != Mem) {
        if (! strcmp(reg_contents[inst->rs1 >>1], "")) {
            st->vj = reg_names[inst->rs1 >>1];
        } else {
            st->qj = reg_contents[inst->rs1 >>1];     
        }

        if (! strcmp(reg_contents[inst->rs2 >>1], "")) {
            st->vk = reg_names[inst->rs2 >>1];
        } else {
            st->qk = reg_contents[inst->rs2 >>1];     
        }
    }
}

PRIVATE bool _ready(Station* st)
{
    if (st->vj != NULL && st->vk != NULL) {
        return true;
    }
    return false;
}

PRIVATE void _propagate_result(SList* stations, Station* cdb)
{
    for (size_t i = 0; i < stations->count; i++) {
        Station* st = &stations->data[i];

        if (st->qj) {
            if (!strcmp(st->qj, cdb->name)) {
                st->vj = cdb->name;
                st->qj = NULL;
            }
        }

        if (st->qk) {
            if (!strcmp(st->qk, cdb->name)) {
                st->vk = cdb->name;
                st->qk = NULL;
            }
        }
    }
}

PRIVATE void _clear_station(Station* st)
{
    st->vj = NULL;
    st->vk = NULL;
    st->busy = false;
}

PRIVATE void _print_scoreboard(InstList* program)
{
    puts("|---------------------------------------------------------------------|");
    puts("| Instruction         | Issue     | Execute   | Writeback | Retired   |");
    puts("|---------------------------------------------------------------------|");
    for (size_t i = 0; i < program->count; i++) {
        inst_show(&program->data[i]);
    }
    puts("|---------------------------------------------------------------------|");
    puts("");
}

PRIVATE void _print_stations(SList* stations)
{
    puts("|---------------------------------------------------------------------|");
    puts("| Reservation stations                                                |");
    puts("|---------------------------------------------------------------------|");
    puts("| Name     |  Busy  |    Op   |   Vj    |    Vk   |    Qj   |    Qk   |");
    puts("|---------------------------------------------------------------------|");
    for (size_t i = 0; i < stations->count; i++) {
        station_show(&stations->data[i]);
    }
    puts("|---------------------------------------------------------------------|");
    puts("");
}

PRIVATE void _print_registers(char* regs[], size_t num)
{
    puts("|-----------------------------------------------------------------------|");
    puts("| Register wait queues (Qi)                                             |");
    puts("|-----------------------------------------------------------------------|");
    puts("|   F0   |   F2   |   F4   |   F6   |   F8   |  F10   |  F12   |  F14   |");
    for (size_t i = 0; i < num; i++) {
        printf("|%7s ", regs[i]);
    }
    puts("|");
    puts("|-----------------------------------------------------------------------|");
}

void retire(Ctx* s, char* reg_contents[])
{
    for(size_t i = 0; i < s->program->count; i++) {
        Inst* inst = &s->program->data[i];

        if (inst->writeback && !inst->retired && inst->writeback != s->cycle) {
            inst->retired = s->cycle;
            reg_contents[inst->rd >> 1] = "";
        }
    }
}

void writeback(Ctx* s)
{
    for (size_t i = 0; i < s->stations->count; i++) {
        Station* st = &s->stations->data[i];
        if (st->busy) {
            if (!st->op->remaining) {
                st->op->writeback = s->cycle;
                _propagate_result(s->stations, st);
                _clear_station(st);
            }
        }
    }
}

void execute(Ctx* s)
{
    for(size_t i = 0; i < s->stations->count; i++) {
        Station* st = &s->stations->data[i];

        if (st->busy) {
            if ((st->op->class == Mem) || _ready(st)) {
                if (st->op->issue != s->cycle) {
                    if (!st->op->execute) {
                        st->op->execute = s->cycle;
                    } else {
                        st->op->remaining--;
                    }
                }
            }
        }
    }
}

void issue(Ctx* s, char* reg_names[], char* reg_contents[])
{
    Inst* inst;
    Station* st;
    size_t i = 0;

    while(i <= s->program->count) {
        inst = NULL;
        st = NULL;

        if (s->program->data[i].issue == 0) {
            inst = &s->program->data[i];
            
            st = _find_station(inst, s->stations);
            if (!st) {
                continue;   
            }

            _fill_station(st, inst, reg_names, reg_contents);
            inst->issue = s->cycle;
            return;
        }
        i++;
    }
    return;
}

void ctx_show(Ctx* s, char* reg_names[])
{
    printf("Cycle : %d \n", s->cycle);
    _print_scoreboard(s->program);
    _print_stations(s->stations);
    _print_registers(reg_names, s->regfile_size);
}
