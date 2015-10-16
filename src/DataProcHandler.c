#include "states.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "DataProcHandler.h"

static int32_t binInstr = 0;
static const int immediatePos = 25;
static const int SPos = 20;
static const int rnPos = 16;
static const int rdPos = 12;
static const int rotatePos = 8;
static const int shiftTypePos = 5;
static const int condPos = 28;
static int immediate = 0;
static const int MAX_EIGHT_BIT = 255;
static const int MAX_COUNTER = 15;

uint32_t rotateRTwo(uint32_t exp) { //Rotate right twice to for operand immediate check
	int lastTwo = checkBin(exp, 0, 1);
	exp >>= 2;
	exp |= lastTwo << 30;
	return exp;
}

//-------OperandHandler for bit 0 to 11-------------------//

uint32_t operandHandler(char *operand, char *shift, char *reg) {
	int imm = 0; //set initial values to zero
	int counter = 0;
	int rm = 0;
	int rs = 0;
	if(operand[0] == '#') { //Operand 2 is an expression
		immediate = 1 << immediatePos;
		if(operand[2] == 'x') {
			imm = getHex(operand + 1); // #0x...hex
		} else {
			imm = atoi(operand + 1); //imm = decimal representation
		}
		if(imm <= MAX_EIGHT_BIT) { //if immediate is already < 255, no need for rotation
			return imm;
		}
    //rotate while immediate is >255 and the number of rotation is less than 15	
    
		while((imm > MAX_EIGHT_BIT && counter < MAX_COUNTER) ||
		      (rotateRTwo(imm) <= MAX_EIGHT_BIT && counter < MAX_COUNTER)) {
		      
      /*if the number have to be rotated to be represented, it will be rotated as much
        as possible */
        
			imm = rotateRTwo(imm);
			counter++;
		}
		
		if(imm > MAX_EIGHT_BIT) { // if imm > 255 after rotating more than 
			perror("Error: Number can't be represented");    // 15 times, then the number is not supported
			exit(EXIT_FAILURE);
		}
		counter = 16 - counter; //0 instead of 16 so when counter = 0, it won't be 10000(bin)
		if(counter == 16) {counter = 0;}
		counter <<= rotatePos;
		return (uint32_t)counter + imm;
	} else { //Operand 2 is a register
		immediate = 0;
		if(shift == NULL) { // in cpsr cases
			return atoi(operand + 1);
		}
		int shiftType = 0;
		rm = atoi(operand + 1);
		char *shiftname = shift; //separating shift type and reg/const
		char *exp = reg;
		if(!strcmp(shiftname, "lsl")) {
			shiftType = 0;
			
		} else if(!strcmp(shiftname, "lsr")) {
			shiftType = 1 << shiftTypePos; //01
			
		} else if(!strcmp(shiftname, "asr")) {
			shiftType = 2 << shiftTypePos; //10
			
		} else if(!strcmp(shiftname, "ror")) {
			shiftType = 3 << shiftTypePos; //11
			
		} else {
			printf("Shift Type invalid");
		}
		
		if(exp == NULL) {exp = reg;}
		
		if(exp[0] == '#') {
		
			if(exp[2] == 'x') {
				imm = getHex(exp + 1); //get hex value
			} else { 
				imm = atoi(exp + 1); //get decimal value
			}
			imm <<= rotatePos - 1; //integer position when shift by a constant amount
			
		} else {
			shiftType += 1 << (shiftTypePos - 1);
			rs = atoi(exp + 1);
			rs <<= rotatePos; //shift specified by register
		}
		
		return rm + rs + shiftType + imm;
	}
}

//------------Handler for and, eor, sub, rsb, add, orr ---------------------//

uint32_t computeHandler(char **tokens) {
	binInstr = 14 << condPos; //1110 at cond
	int rd = atoi(tokens[1] + 1) << rdPos; //tokens[1] + 1 to get the number after r (rx, to get x)
	int rn = atoi(tokens[2] + 1) << rnPos;
	int opCodeS = 0;
	
	if(!strcmp(tokens[0], "and")) {
		opCodeS = 0; //Opcode = 0000
	}
	
	if(!strcmp(tokens[0], "eor")) {
		opCodeS = 2 << SPos; //Opcode = 0001, S = 0 so 00010 = 2
	}
	
	if(!strcmp(tokens[0], "sub")) {
		opCodeS = 4 << SPos; //Opcode = 0010, S = 0 so 00100 = 4
	}
	
	if(!strcmp(tokens[0], "rsb")) {
		opCodeS = 6 << SPos; //Opcode = 0001, S = 0 so 00110 = 6
	}
	
	if(!strcmp(tokens[0], "add")) {
		opCodeS = 8 << SPos; //Opcode = 0001, S = 0 so 01000 = 8
	}
	
	if(!strcmp(tokens[0], "orr")) {
		opCodeS = 24 << SPos;//Opcode = 0001, S = 0 so 11000 = 24
	}

	uint32_t operand = operandHandler(tokens[3], tokens[4], tokens[5]);
	binInstr += immediate + opCodeS + rn + rd + operand;
	return binInstr;
}

//--------Handler for mov ----------------//

uint32_t movHandler(char **tokens) {

	binInstr = 14 << condPos; //1110 at cond
	int rd = atoi(tokens[1] + 1) << rdPos;
	int opCodeS = 26 << SPos; // Opcode = 1101, S = 0 so 11010 = 26
	uint32_t operand = operandHandler(tokens[2], tokens[3], tokens[4]);
	binInstr += immediate + opCodeS + rd + operand;
	return binInstr;
}

//-----------Handler for tst, teq, cmp-------------//

uint32_t cpsrHandler(char **tokens) {

	binInstr = 14 << condPos; //1110 at cond
	int rn = atoi(tokens[1] + 1) << rnPos;
	int opCodeS = 0;
	if(!strcmp(tokens[0], "tst")) {
		opCodeS = 17 << SPos; //Opcode = 1000, S = 1 so 10001 = 17
	}
	
	if(!strcmp(tokens[0], "teq")) {
		opCodeS = 19 << SPos; //Opcode = 1001, S = 1 so 10011 = 19
	}
	
	if(!strcmp(tokens[0], "cmp")) {
		opCodeS = 21 << SPos; //Opcode = 1010, S = 1 so 10101 = 21
	}
	
	uint32_t operand = operandHandler(tokens[2], NULL, NULL);
	binInstr += immediate + opCodeS + rn + operand;
	return binInstr;
}
