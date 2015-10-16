#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>
#include "states.h"
#include "pipeline.h"
#include "util.h"
#include "dataprocessing.h"
#include "multiply.h"
#include "SingleDataTransfer.h"

const int NUM_OF_REG = 17;
const int INSTR_SIZE = 4; //4 bytes
const int MEM_SIZE = 65536;   // 2^16


void printNZAux(char* memPtr);
void execute(struct pipeline *pipe, struct state *st, char *memPtr);

//----------------------------------------MAIN------------------------------------------------------
int main(int argc, char **argv) {
	struct state s;
	struct state *sPtr = &s;
	for(int i = 0; i < NUM_OF_REG; i++) { //initialize registers
		sPtr -> reg[i] = 0;
	}
	FILE *file = fopen(argv[1], "rb" );
	fseek(file, 0, SEEK_END);
	int fileSize = ftell(file); //number of bytes
	fseek(file, 0, SEEK_SET);
	
	int maxSize = fileSize/INSTR_SIZE; //max number of instructions
	char *memPtr = calloc(MEM_SIZE, CHAR_BIT); //memPtr is the pointer to the instructions in the heap
	char *tempPtr = memPtr;
	unsigned char buffer[sizeof(uint32_t)];
	for(int i = 0; i < maxSize; i++){
		int count = fread(&buffer[0], sizeof(buffer), 1, file);//read into buffer the 32bit instructions
		if(count < 1) return 0; //check for fread
		for(int j = 0; j < sizeof(buffer); j++) {
			*tempPtr = buffer[j]; //allocating it byte by byte
			tempPtr++;
		}
	}

	struct pipeline p;
	struct pipeline *pPtr = &p;
	sPtr -> reg[15] = 0; //reg[15] = PC = 0
	pPtr -> fetched = *((uint32_t*)(memPtr + sPtr -> reg[15]));
	sPtr -> reg[15] += 4;

	pPtr -> decoded = pPtr -> fetched;
	pPtr -> fetched = *((uint32_t*)(memPtr + (sPtr -> reg[15])));
	sPtr -> reg[15] += 4;

	int terminated = 0;
	if (pPtr -> decoded == 0) {
	pPtr -> decoded = pPtr -> fetched;
	terminated = 1; //if no more decoded instruction, terminate
	}
	
	while (terminated == 0) {
		execute(pPtr, sPtr, memPtr); //execute the first instruction
		pPtr -> decoded = pPtr -> fetched; //fetched becomes decoded
		pPtr -> fetched = *((uint32_t*)(memPtr + (sPtr -> reg[15]))); //fetch the new instruction
		sPtr -> reg[15] += 4; //increment PC
		if (pPtr -> decoded == 0) terminated = 1;
	}

	printf("Registers:\n");
	for(int i = 0; i < 13; i++) {//printing out register 0-12
		int x = sPtr->reg[i];
		printf("$%-3i: %10i (0x%08x)\n", i, x, x);
	}
	int y = sPtr->reg[15];	 //printing out PC
	printf("PC  : %10i (0x%08x)\n", y, y);
	y = sPtr->reg[16]; //printing out CPSR
	printf("CPSR: %10i (0x%08x)\n", y, y);
	printf("Non-zero memory:\n"); //Printing out memory
	for(int i = 0; i < MEM_SIZE; i++) {
		if(*((uint32_t *)(memPtr + INSTR_SIZE * i)) != 0) {
			printf("0x%08x: ", INSTR_SIZE * i);
       			printNZAux(memPtr + INSTR_SIZE * i);
			printf("\n");
		}
	}	


	free(memPtr);
	fclose(file);
 
	return 0;
}

void printNZAux(char* memPtr){
	printf("0x");
	for(int i = 0; i < INSTR_SIZE; i++) {
		printf("%02hhx", *(memPtr + i));
	}
}

//---------------------------------------------------------------------------------------
void branch(uint32_t i, struct state *s) {

	int32_t offset = checkBin(i, 0, 22);
	offset <<=2;
	if (checkBin(i, 23, 23)) {offset -= 33554432;}
	s -> reg[15] += offset;
	return ;
	

}

//----------Execute decoded instruction---------------------------------//

void execute(struct pipeline *pipe, struct state *st, char *memPtr) {

	uint32_t instr = pipe -> decoded;

	// check for termination
	if (instr == 0) return;

	//check cond, do nothing and return if not satisfied
	int cond = checkBin(instr, 28, 31);
	int cpsr = st -> reg[16];

		//  N = 31  //  Z = 30  //  C = 29  //  V = 28
	switch (cond) {
	case 0  : if ((checkBin(cpsr, 30, 30)) == 0) {
		  return; }
		  break;
	case 1  : if (checkBin(cpsr, 30, 30) == 1) {
		  return; }
		  break;
	case 10 : if (checkBin(cpsr, 31, 31) != checkBin(cpsr, 28, 28)) {
		  return; }
		  break;
	case 11 : if (checkBin(cpsr, 31, 31) == checkBin(cpsr, 28, 28)) {
		  return; }
		  break;
	case 12 : if (checkBin(cpsr, 30, 30) == 1 || (checkBin(cpsr, 31, 31) != checkBin(cpsr, 28, 28))) {
		  return; }
		  break;
	case 13 : if (checkBin(cpsr, 30, 30) == 0 && (checkBin(cpsr, 31, 31) == checkBin(cpsr, 28, 28))) {
		  return; }
		  break;
	case 14 : break;
	default : return ;
	}

	int head = checkBin(instr, 26, 27); //conditions to determine which method to call
	int body = checkBin(instr, 4, 7);
	
	if (body == 9 && checkBin(instr, 22, 27) == 0) { //check for all 0's between bit 22-27
	 multiply(st, instr);
	 return;
        }
	if (head == 1) {
	 SDTransfer(st, instr, memPtr);
	 return;
        }
	if (head == 2) {
	branch(instr, st);
	pipe -> fetched = *((uint32_t*)(memPtr + (st -> reg[15])));
	st -> reg[15] += 4;
     return;
	}
	if (head == 0) {
  	 dataprocess(instr, st);
	 return;
	}
	printf("Could not decode instruction");
}


