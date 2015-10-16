#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "branchHandler.h"

uint32_t branchHandler(char **token, struct table *tPtr, int address){
  uint32_t result = 0;
  

  //------------------------- finding offset -------------------------// 
  char *label = token[1];
  int len = strlen(label);

  char *label2 = malloc(len +1 +1);  // +1 for new char +1 for \0 
                                     // since strlen doesnt take into account 
                                     // the last \0
  strcpy(label2,label);
  label2[len] = ':';
  label2[len+1] = '\0';

  int labeladdr = table_get_addr(tPtr, label2);
  free(label2);
  int offset = labeladdr - address;
  offset = offset - 2;
                  // because of the pipeline, always 2 instr ahead 

  //------------------------- offsetfound -----------------------------//

     //pc is increment by 4 for each offset, but this is dealt by branch instr
     //itself by shifting left 2 positions. So we don't need to care abt that now
  
  // setting bit 23
  // ------------------------------------------------------------------// 
  if(offset < 0){
    result = setBin(result,1,23,23);
  }
  else{
    result = setBin(result,0,23,23);
  }
  
  result = setBin(result,offset, 0 ,22);  
  
  //set bit 28-31
  //-------------------------------------------------------------------//

  char *mnemonic = token[0];
  if(!strcmp(mnemonic,"b")){
    //cond (bit 28-31) = 1110 = 14
    result = setBin(result, 14, 28, 31); 
  }
  else if(!strcmp(mnemonic,"beq")){
    //0000 = 0
    result = setBin(result, 0, 28, 31);
  }
  else if(!strcmp(mnemonic,"bne")){
    //0001 = 1
    result = setBin(result,1,28,31);
  }
  else if(!strcmp(mnemonic,"bge")){
    //1010 = 10
    result = setBin(result,10,28,31);
  }
  else if(!strcmp(mnemonic,"blt")){
    //1011 = 11
    result = setBin(result, 11, 28, 31);
  }
  else if(!strcmp(mnemonic,"bgt")){
    //1100 = 12
    result = setBin(result, 12, 28, 31);
  }
  else if(!strcmp(mnemonic,"ble")){
    //1101 = 13
    result = setBin(result, 13, 28, 31); 
  }
  else{
    perror("wrong mnemonic passed to branch handler");
    exit(EXIT_FAILURE);
  }

    // setting bit 24-27
  //------every branch instruction = write bit 24-27 as 1010----------//
  result = setBin(result,10,24,27);

  return result; 
}

