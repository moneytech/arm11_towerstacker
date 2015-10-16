#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdint.h>
#include "util.h"
#include "states.h"
#include "dataprocessing.h"

  
void setCPSR(uint32_t result, int Sbit, struct state *sPtr){
  if(Sbit == 1){
    if(result == 0){
      setZBit(1, sPtr);
    } else {
	  setZBit(0, sPtr);
	}
    setNBit(checkBin(result,31,31), sPtr); 
  }
}

// ------------------- barrelShifter functions ---------------------------//

uint32_t barrelShifter(int type, uint32_t value, int amount, int Sbit, 
                       struct state *sPtr){
  
  typedef uint32_t (*shift)(uint32_t value, int amount, int Sbit, struct state *sPtr);
  shift fpointers[] = {lsl, lsr, asr, ror};
  
  if (type > 3) {perror("invalid type for barrelShifter"); exit(EXIT_FAILURE); }
  return fpointers[type](value, amount, Sbit, sPtr);
}


//rotate right
uint32_t ror(uint32_t value, int amount, int Sbit, struct state *sPtr){
  uint32_t rotated = 0;
  for(int i = 31; i >= 0; i--){
    rotated <<= 1;
    int pos = (i+32+amount)%32; 
    rotated |= checkBin(value, pos, pos);
    if(Sbit == 1){
      setCBit(checkBin(rotated,31,31), sPtr);
    }
  } 
  return rotated;
}

//logical shift left
uint32_t lsl(uint32_t value, int amount, int Sbit, struct state *sPtr){
  uint32_t shifted = value << amount;
  if(amount != 0){
    int carry = checkBin(value, 32-amount, 32-amount);
    if(Sbit == 1){
      setCBit(carry, sPtr);
    } 
  }
  return shifted;
}

//logical shift right
uint32_t lsr(uint32_t value, int amount, int Sbit,  struct state *sPtr){
  uint32_t shifted = value >> amount;
  if(amount != 0){
    int carry = checkBin(value,amount-1,amount-1);
    if(Sbit == 1){
      setCBit(carry,sPtr);
    }
  }
  return shifted;
}

//arithmetic shift right
uint32_t asr(uint32_t value, int amount, int Sbit, struct state *sPtr){
  uint32_t shifted = 0;
  for(int i = 0; i < amount; i++){
    shifted = shifted << 1;
    shifted |= 1;
  }
  for(int i = 31; i >= amount; i--){
    shifted |= checkBin(value,i,i); 
  }
  int carry = checkBin(value,amount-1, amount-1); 
  if(Sbit == 1){
    setCBit(carry,sPtr);
  }
  return shifted;    
}

//-----------------------------------------------------------------------//
//------------------  arithmatic and logical functions ------------------//
uint32_t and(uint32_t in1, uint32_t in2){
  uint32_t result = 0;
  for(int i = 31; i >= 0; i--){
    result <<= 1;
    int anded = checkBin(in1,i,i) & checkBin(in2,i,i);
    result |= anded;
  }
  return result;
}

uint32_t eor(uint32_t in1, uint32_t in2){
  uint32_t result = 0;
  in2 = ~(in2);
  for(int i = 31; i>=0; i--){
    result <<= 1;
    int eored = checkBin(in1,i,i) == checkBin(in2,i,i); 
    result |= eored;
  }
  return result;
}

uint32_t orr(uint32_t in1, uint32_t in2){
  uint32_t result = 0;
  for(int i = 31; i >= 0; i--){
    result <<= 1;
    int orred = checkBin(in1,i,i) | checkBin(in2,i,i);
    result |= orred;
  }
  return result;
}

uint32_t sub(uint32_t in1, uint32_t in2, int Sbit, struct state *sPtr){
  uint32_t result = in1 - in2;
  int borrow = result > in1;
  if(Sbit == 1){
    setCBit(1-borrow, sPtr);
  }
  return result;
}

uint32_t add(uint32_t in1, uint32_t in2, int Sbit, struct state *sPtr){
  uint32_t result = in1 + in2;
  int carry = checkBin(in1,31,31) & checkBin(in2,31,31);
  if(Sbit == 1){
    setCBit(carry, sPtr);
  }
  return result;
}
// -------------------------------------------------------------------//

// ----------------------- the main function -------------------------//
//
// given the instruction and the current state of the machine
// carries out the function, and updated the state of the machine.

void dataprocess(uint32_t inst, struct state *sPtr){
  //check 'I' bit (bit 25)
  int rn = checkBin(inst,16,19);
  uint32_t rnValue = sPtr -> reg[rn];
  int Sbit = checkBin(inst, 20,20);
  int rd = checkBin(inst,12,15);
  uint32_t result = sPtr -> reg[rd];                    
  uint32_t oprand2;

  if(checkBin(inst,25,25) == 1){
    int rotation = checkBin(inst,8,11);
    uint32_t imm = checkBin(inst,0,7);
    oprand2 = ror(imm,rotation*2,0,sPtr); 
  }
  else{
    int rm = checkBin(inst, 0,3);
    uint32_t rmValue = sPtr -> reg[rm];
    int stype = checkBin(inst, 5,6);
    if(checkBin(inst,4,4) == 0){
      int shiftamount = checkBin(inst,7,11);
      oprand2 = barrelShifter(stype, rmValue, shiftamount, Sbit, sPtr);
    }
    else{
      int rs = checkBin(inst,8,11);
      int shiftamount = checkBin(sPtr -> reg[rs],0,7);
      oprand2 = barrelShifter(stype, rmValue, shiftamount, Sbit, sPtr);
    }

  }
  int opcode = checkBin(inst,21,24);

    switch(opcode){
      case 0: // and
        result = and(rnValue, oprand2);
        setCPSR(result,Sbit, sPtr);
        break;
      case 1: // eor
        result = eor(rnValue, oprand2);
        setCPSR(result,Sbit, sPtr);
        break;
      case 2: // sub
        result = sub(rnValue, oprand2, Sbit, sPtr);
        setCPSR(result,Sbit,sPtr);
        break;
      case 3: // rsb
        result = sub(oprand2, rnValue, Sbit, sPtr);
        setCPSR(result,Sbit,sPtr);
        break;
      case 4: // add
        result = add(rnValue, oprand2, Sbit, sPtr);
        setCPSR(result,Sbit,sPtr);
        break;
      case 8: // tst
        setCPSR(and(rnValue, oprand2), Sbit, sPtr);
        break;
      case 9: // teq
        setCPSR(eor(rnValue, oprand2), Sbit, sPtr);
        break;
      case 10: // cmp
        setCPSR(sub(rnValue, oprand2, Sbit, sPtr), Sbit, sPtr);
	break;
      case 12: // orr
        result = orr(rnValue, oprand2);
        setCPSR(result,Sbit,sPtr);
        break;
      case 13: // mov
        result = oprand2;
        setCPSR(result, Sbit, sPtr);
        break;
      default:
        perror("wrong opcode fed to dataprocessor");
        exit(EXIT_FAILURE);
    }    

  sPtr-> reg[rd] = result;    
}

