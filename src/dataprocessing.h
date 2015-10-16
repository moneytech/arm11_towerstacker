#ifndef DATAPROCESSING_H
#define DATAPROCESSING_H
#include "util.h"
uint32_t barrelShifter(int type, uint32_t value, int amount, int Sbit, 
                       struct state *sPtr);

uint32_t lsl(uint32_t type , int amount, int Sbit, struct state *sPtr);
uint32_t lsr(uint32_t type , int amount, int Sbit, struct state *sPtr);
uint32_t asr(uint32_t type , int amount, int Sbit, struct state *sPtr);
uint32_t ror(uint32_t type , int amount, int Sbit, struct state *sPtr);

void dataprocess(uint32_t inst, struct state *sPtr);

uint32_t and(uint32_t in1, uint32_t in2);
uint32_t eor(uint32_t in1, uint32_t in2);
uint32_t orr(uint32_t in1, uint32_t in2);
uint32_t sub(uint32_t in1, uint32_t in2, int Sbit, struct state *sPtr);
uint32_t add(uint32_t in1, uint32_t in2, int Sbit, struct state *sPtr);

#endif

