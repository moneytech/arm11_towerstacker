#include "sdtHandler.h"
#include "util.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "DataProcHandler.h"

/*
  PRE:  ts: is the passed set of tokens from main
	address: is the address of current instruction
	outBuffer: points to the output buffer to be written to the file 
		so that we can write constant to end of while (load with const case)
	finaloutputnum: points to the calculated number of total output for
		this set of instructions in main, use for load with const also
	outputnum: points to current outputnum in main, use to increment the outnum
		in load with const case

  POST: this should return the 32 bit instr corresponding to the store and load assembly instuction
*/

uint32_t sdtHandler(char** ts, int address, uint32_t **outBuffer, int *finalOutputNum, int *outputNum){

	uint32_t result = 0;
	char* ldrstrval = ts[2];
	result = setBin(result, 1, 26, 27);
	char *label = ts[0];
	result = setBin(result, 14, 28, 31);  //set to ignore CPSR
	int tssize = 0;
	for (int i = 0; ts[i] != NULL; i++){
	tssize++;
	}
	printf("no of ts = %i \n", tssize);
//	determine load or store------------------------------------

	if (!(strcmp(label, "ldr"))) {
	result = setBin(result, 1, 20, 22);  // set Load
	}

// 	set the dest reg Rd -----------------------------------------------

	char *dest = ts[1];
	int destregnum = atoi(dest+1);
	result = setBin(result, destregnum, 12, 15);

//	determine form of <address>--------------------------------------------------

// 	load with const expression ---------------------------------------------------

	if(ldrstrval[0] == '=') { 

	int constant;
	sscanf((ldrstrval+1), "%i", &constant);

			if (( constant <= 0xFF)) { //small const so treat as mov

			result = setBin(result, 29, 21, 25);
			result = setBin(result, constant, 0, 11);
			result = setBin(result, 0, 26, 26);     //previously set these to 1 
			result = setBin(result, 0, 20, 20); 

			} else {  //constant big so have to store at the end

			result = setBin(result, 3, 23, 24);    //set P and U --doesnt affect res
			int pc = 15 ;                         //pc always 15 
			int valaddr = *finalOutputNum;
			(*outBuffer)[valaddr] = constant;
			int offset = (valaddr - address - 2)*4; 
	
			result = setBin(result, offset, 0, 11);
			result = setBin(result, pc, 16, 19); // pc = base reg in this case
			*outputNum = *outputNum + 1;
			*finalOutputNum = *finalOutputNum + 1;

			}

//	store or load with non-const ---------------------------------------------

	} else { 
		char* lastts = ts[tssize -1];
		//if (strcmp(lastts + (strlen(lastts)-1), "]") != 0) { 
		if (strchr(lastts, ']') != NULL) {
		//check if last char is a ']' -> pre indexing 
		result = setBin(result, 1, 24, 24);
		printf("pre index \n");
		} 

	//set base reg
	char* basereg = ts[2];
	int baseregnum = atoi(basereg + 2);
	result = setBin(result, baseregnum, 16, 19);

		//no offset in this case 
		if (ts[3] == NULL) {
		result = setBin(result, 1, 24, 24);
		result = setBin(result, 1, 23, 23);
		return result;
		}

	char* shiftby = ts[3];

		if (shiftby[0] == '#') { //constant offset 

			if (shiftby[1] == '-') {
			shiftby++;
			} else {
			result = setBin(result, 1, 23, 23);   //set the up bit for add
			}

		char* hexidentifier = "0x";
		int offset = 0;

			if (strstr(shiftby, hexidentifier) != NULL) { 
			//check if the const is in hex format
			sscanf((shiftby + 1), "%i", &offset);
			} else {
			offset = atoi(shiftby + 1);
			}
			printf("offset = %i \n", offset);
			result = setBin(result, offset, 0, 11);
			return result;

		//support for register offset ------------------------------
		} else { 

		result = setBin(result, 1, 25, 25);
		int regisnumber = 0;

			if (shiftby[0] != '-') {

			result = setBin(result, 1, 23, 23);   //set the up bit for add
			regisnumber = atoi(shiftby + 1);

			} else {
			regisnumber = atoi(shiftby + 2);

			}

		result = setBin(result, regisnumber, 0, 3);	

		//support for SHIFTED register offset ---------------------------------	
	
			if (ts[4] != NULL) {
				//determine the shifting type
				//no lsl as the bits are already 0s

				if ( !(strcmp(ts[4], "lsr"))) { 
				result = setBin(result, 1, 5, 6);
				} 

				if ( !(strcmp(ts[4], "asr"))) {
				result = setBin(result, 2, 5, 6);
				} 

				if ( !(strcmp(ts[4], "ror"))) {
				result = setBin(result, 3, 5, 6);
				}  

			char* shiftby = ts[5];

				if (shiftby[0] == '#') {
				//shift by const amount				

				int shiftbynum = atoi(shiftby +1);
				result = setBin(result, shiftbynum, 7, 11);

				} else {
				//shift by another register

				result = setBin(result, 1, 4, 4);
				int shiftbyreg = atoi(shiftby + 1);
				result = setBin(result, shiftbyreg, 8, 11);	
	
				}
			}

		}
		

	}
	return result;
}

