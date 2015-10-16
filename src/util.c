#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "util.h"
#include "states.h"
#include "assert.h"
#include <string.h>

int checkBin(uint32_t bin, int from, int to){
  uint32_t mask = -1; //   =0xffffffff  // all 32 bits = 1 
  mask = mask >> (31 - (to-from)); 
  mask = mask << from;
  bin = bin & mask;
  bin = bin >> from;
  return bin; 
}
//this function checks the binary value of 'uint32_t bin' at position 'from' to 'to'
//checkBin(x,0,0) checks bit 0 of x 

void printBin(uint32_t bin){
  printf("binary of %u = ",bin);
  for(int i = 31; i >= 0; i--){
    printf("%i",checkBin(bin,i,i));
  }
  printf("\n");
}

//when print out binary of int32_t value -1 will give of the same
// value as 0xffffffff why is this? uint32_u has negative value?
//
// A: giving negative value to unsigned int, it will converted the 
//    negative value to it's positive 2-complement

//Setting the different flag bits in CPSR
void setNBit(int value, struct state *sPtr) {
  sPtr->reg[16] &= ~(1 << 31);
  sPtr->reg[16] |= value << 31;
}

void setZBit(int value, struct state *sPtr) {
  sPtr->reg[16] &= ~(1 << 30);
  sPtr->reg[16] |= value << 30;
}

void setCBit(int value, struct state *sPtr) {
  sPtr->reg[16] &= ~(1 << 29);
  sPtr->reg[16] |= value << 29;
}

void setVBit(int value, struct state *sPtr) {
  sPtr->reg[16] &= ~(1 << 28);
  sPtr->reg[16] |= value << 28;
}

uint32_t endianFlip(uint32_t code){
  uint32_t result = 0;
  uint32_t mask = 0xff;
  for(int i = 0; i< 4; i++){
    uint32_t immediate = code & mask;
    result <<= 8;
    result |= immediate;
    printBin(result);
    code >>= 8;
  }
  return result;

}

/*	
Similar to checkBin above where the bits in instr between from 
and to are changed to value
So if value = 12, then the bits between from and to in instr 
will be set to 1100
*/

uint32_t setBin(uint32_t instr, int value, int from, int to) {
	uint32_t mask = 0xffffffff;
	mask >>= 31 - (to - from);
	value &= mask; //set all 1s before the value if -ve to 0
	mask <<= from;
	instr &= ~mask; // clear the bits between from and to
	value <<= from;
	instr |= value;
	return instr;
}

int getHex(char *hexString) {
	uint32_t hex;
	sscanf(hexString, "%x", &hex);
	return hex;
}

//removes tab and spaces and replace with new line if no instructions detected
void removeFrontSpaces(char **token) {
  while(**token == '\t' || **token == ' ') {
    (*token)++;
  }
}

void removeBackSpaces(char **token) {
  int pos = strlen(*token);
  while((*token)[pos] == '\t' || (*token)[pos] == ' ') {
    (*token)[pos] = '\0';
    pos--;
  }
}

int checkComments(char *token) {
  char *string = token;
  while(*string != '\0') {
    if((*string == '/' || *string == '*' ) && *(string + 1) == '/') {
      return 1;
    }
    string++;
  }
  return 0;
}

void removeComments(char **token) {
  int pos = strlen(*token);
  while((*token)[pos] != '/') {
    (*token)[pos] = '\0';
    pos--;
  }
  while((*token)[pos] == '/') {
    (*token)[pos] = '\0';
    pos--;
  } 
}
