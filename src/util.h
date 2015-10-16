#ifndef UTIL_H
#define UTIL_H
#include "states.h"
int checkBin(uint32_t bin, int from, int to);

//this function checks the binary value of 'uint32_t bin' at position 'from' to 'to'
//checkBin(x,0,0) checks bit 0 of x 

void printBin(uint32_t bin);

//when print out binary of int32_t value -1 will give of the same
// value as 4294967295 why is this? uint32_u has negative value?
//
// A: giving negative value to unsigned int, it will converted the 
//    negative value to it's positive 2-complement


void setNBit(int value, struct state *sPtr);
void setZBit(int value, struct state *sPtr);
void setCBit(int value, struct state *sPtr);
void setVBit(int value, struct state *sPtr);

uint32_t endianFlip(uint32_t code);
uint32_t setBin(uint32_t instr, int value, int from, int to);
int getHex(char *hexString); //returns value of hexString

void removeFrontSpaces(char **token);
void removeBackSpaces(char **token);
int checkComments(char *token);
void removeComments(char **token);
#endif

