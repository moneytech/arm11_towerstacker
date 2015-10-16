#include <stdio.h>
#include <string.h>
#include "firstpass.h"

//Go through all instructions, and if it is a label, insert it into the 
//table. Returns number of lables found in the instructions given.
int firstPass(char **instrPtr, struct table *tPtr){
  
  int addr = 0;
  int realAddr = 0;

  int numLabel = 0;
  while(instrPtr[addr] != NULL){
    if(isLabel(instrPtr[addr])){
      table_insert_front(tPtr, instrPtr[addr], realAddr); 
      numLabel++;
    }
    else{
      realAddr++; 
    }
    addr++;
  } 
  return numLabel;
}


int isLabel(char *str){
  if(strchr(str,':') != NULL){
    return 1;
  }
  else{
    return 0;
  }
}


