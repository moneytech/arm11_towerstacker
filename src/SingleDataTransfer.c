#include <stdlib.h>
#include <stdio.h>
#include "util.h"
#include "SingleDataTransfer.h"
#include "states.h"
#include "dataprocessing.h"

/*
  PRE:  *s: is the pointer to the current states (registers)
	instr: is the 32 bit instruction to be executed
	*memPtr: points to the memory (calloc in main) so can load/store

  POST: this should set the registers/ load/ store values as specified in the instr
*/

void SDTransfer(struct state *s, uint32_t instr, char *memPtr){

	int im_offset = checkBin(instr, 25, 25);
	int prepost   = checkBin(instr, 24, 24);
	int up        = checkBin(instr, 23, 23);
	int loadstore = checkBin(instr, 20, 20);	// 1 load, 0 store
	int regnbits  = checkBin(instr, 16, 19);
	uint32_t basereg   = s -> reg[regnbits];           //base register, Rn
	int srcdest   = checkBin(instr, 12, 15);
	uint32_t offset = 0;
	int memaddr   = basereg ;               //compute by base reg and offset - prepost

	//------------------determine if offset is immediate or shifted reg-----------------

	int regshift = 17;
	//need to set to 17 for checking **CHECK 1** (ie 17 is impossible)
	if (im_offset){
	int regshiftbits = checkBin(instr, 0, 3);
	regshift = s -> reg[regshiftbits];         //reg to be shifted , Rm
	int shiftvaluebit = checkBin(instr, 4, 4);
	int shifttype = checkBin(instr, 5, 6);
	int shiftamount;
		// get the shift value
		if (shiftvaluebit) {
		int shiftamountregbits = checkBin(instr, 8, 11);
		shiftamount = s -> reg[shiftamountregbits]; 
		} else {
		shiftamount = checkBin(instr, 7, 11);
		}
	offset = barrelShifter(shifttype, regshift, shiftamount, 0, s);
	
	} else {
	offset = checkBin(instr, 0, 11);
	}

	//------------------carry out pre indexing if set----------------------- 

	if (prepost) {

		if (up) {
		memaddr += offset;
		} else {
		memaddr -= offset;
		}

	} else {
	//to make sure we dont allow post indexing ldr or str with regshift = basereg **CHECK 1**
		if (basereg == regshift) {
		return;
		}
	}


	// -----------------------carry out the load or store -----------------

	//pin accessing or case where memory is out of bounds
	if (memaddr > 65536 || memaddr < -65536) {
	switch (memaddr) {
	case 538968064 : printf("One GPIO pin from 0 to 9 has been accessed\n");
			if (loadstore) {
			s -> reg[srcdest] = (memaddr);
			} break;   // hex 20200000 	
	case 538968068 : printf("One GPIO pin from 10 to 19 has been accessed\n");
			if (loadstore) {
			s -> reg[srcdest] = (memaddr);
			} break; // hex 20200004
	case 538968072 : printf("One GPIO pin from 20 to 29 has been accessed\n"); 
			if (loadstore) {
			s -> reg[srcdest] = (memaddr);
			} break; // hex 20200008 
	case 538968104 : printf("PIN OFF\n"); break;         //hex 20200028
	case 538968092 : printf("PIN ON\n"); break;         //hex 2020001c
	default : printf("Error: Out of bounds memory access at address 0x%08x\n", memaddr);
	}
	return;
	}

	//general load/store cases

	if (loadstore) {
	s -> reg[srcdest] = *((uint32_t*)(memPtr + memaddr));

	} else {
	*((uint32_t*)(memPtr + memaddr)) = s -> reg[srcdest];

	}

	// ----------------------------post indexing------------------------------- 

	if (!(prepost)) {

		if (up) {
		s -> reg[regnbits] = basereg + offset ;

		} else {
		s -> reg[regnbits] = basereg - offset ;

		}		
	}
}

