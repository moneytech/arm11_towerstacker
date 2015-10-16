#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "MultiplyHandler.h"

static const int condPos = 28;
//static const int accPos = 21;
static const int aPos = 21;
static const int rdPos = 16;
static const int rnPos = 12;
static const int rsPos = 8;
static uint32_t binInstr = 144; // 1001 at bit 4-7

//PRE: tokens: passed set of tokens from main
//POST: return the 32 bit instr corresponding to the assembly mul instuction 

uint32_t mulHandler(char **tokens) {
	int cond = 14 << condPos;
	int rd = atoi(tokens[1] + 1) << rdPos;
	int rm = atoi(tokens[2] + 1);
	int rs = atoi(tokens[3] + 1) << rsPos;
	return binInstr + cond + rd + rm + rs;
}

//PRE: tokens: passed set of tokens from main
//POST: return the 32 bit instr corresponding to the assembly mla instuction 

uint32_t mlaHandler(char **tokens) {
	int cond = 14 << condPos;
	int rd = atoi(tokens[1] + 1) << rdPos;
	int rm = atoi(tokens[2] + 1);
	int rs = atoi(tokens[3] + 1) << rsPos;
	int rn = atoi(tokens[4] + 1) << rnPos;
	int a = 1 << aPos;
	return binInstr + cond + a + rd + rm + rs + rn;
}
