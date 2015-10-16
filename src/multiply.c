#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdint.h>
#include "util.h"
#include "states.h"
#include "multiply.h"

static const int accPos = 21;
static const int setPos = 20;
static const int destFrom = 16;
static const int destTo = 19;
static const int regNFrom = 12;
static const int regNTo = 15;
static const int regSFrom = 8;
static const int regSTo = 11;
static const int regMFrom = 0;
static const int regMTo = 3;

void multiply(struct state *sPtr, uint32_t instr) {

	int acc = checkBin(instr, accPos, accPos);
	int set = checkBin(instr, setPos, setPos);
	int dest = checkBin(instr, destFrom, destTo);
	int opn = checkBin(instr, regNFrom, regNTo);
	int ops = checkBin(instr, regSFrom, regSTo);
	int opm = checkBin(instr, regMFrom, regMTo);

	int regn = sPtr->reg[opn];
	int regs = sPtr->reg[ops];
	int regm = sPtr->reg[opm];

	uint32_t regd = regm * regs; //get the lower 32bits
		
	if(acc) { // if (acc == 1)
		regd += regn;
	}

	sPtr->reg[dest] = regd; //save result in dest register

	if(set) { // if (set == 1)
		setNBit(checkBin(regd, 31, 31), sPtr);
		if(!regd) { // if(regd == 0)
			setZBit(1, sPtr);
		}
	}
}
