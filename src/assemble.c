#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "table.h"
#include "firstpass.h"
#include "DataProcHandler.h"
#include "MultiplyHandler.h"
#include "sdtHandler.h"
#include "branchHandler.h"
#include "util.h"


uint32_t assemble(char** ts, int address, struct table *tPtr, uint32_t **outBuffer, int* finalOutputNum, int* outputNum);   //add the argument address here because i need it for branch offset calculation//

int main(int argc, char **argv) {

	FILE *fp;
	if ((fp = fopen(argv[1], "r")) == NULL) {
	  perror("Error opening file");
	  exit(EXIT_FAILURE);
	}

	fseek(fp, 0, SEEK_END);
	int fileSize = ftell(fp); //number of bytes
	fseek(fp, 0, SEEK_SET);
	
	char *inputPtr = calloc(fileSize, sizeof(char));
	int ret = fread(inputPtr, 1, fileSize, fp);
	if(ret < fileSize) {
		printf("fread faillure");
	}


	char *tempinPtr = calloc(fileSize, sizeof(char));
	strcpy(tempinPtr, inputPtr);

	fclose(fp);

//-----------------------------------------------------------------
//	find out the number of lines for calloc
//	strtok destroys the string, so we did a copy of input

	const char newline[2] = "\n";
	char *token;
	token = strtok(tempinPtr, newline);
	
	int count = 0;	

	while (token != NULL) {
	 removeFrontSpaces(&token);                          //ignores any spaces in the program (util.c)
     if(checkComments(token)) {
      removeComments(&token);
      removeBackSpaces(&token);
     }
	   if(strlen(token) < 3) {  //supports single line comments in the code
	    token = strtok(NULL, newline);
	    continue;
	   } else {
//	   if(strlen(token) >= 3) {
	    token = strtok(NULL, newline);
           // null is passed as first argument because strtok maintains a static 
           // pointer to your previous passed string
	    count++;
	   }
	}
	int const numInstr = count;

	//the last token is always useless so can just ignore

//----------------------------------------------------------------


	char** instrPtr;
	instrPtr = calloc(2 * numInstr, 512 * sizeof(char));
	
	token = strtok(inputPtr, newline);    // get first token
	count = 0;

	while (token != NULL) {
	 removeFrontSpaces(&token);                  //ignores any spaces in the program (util.c)
   if(checkComments(token)) {
     removeComments(&token);
     removeBackSpaces(&token);
   }
	   if(strlen(token) < 3) {   //supports single line comments in the code
	    token = strtok(NULL, newline);
	    continue;
	   }
   printf("%s\n", token);
	 instrPtr[count] = token;
	 token = strtok(NULL, newline);       // "in each subsequent call that should parse the 
                                              //  same string the first arg should be NULL" -man3
	 count++;	 
	 }

	free(tempinPtr);

//	post: instrPtr[i] holds the ith instruction line
//------------------------------------------------------------------

// ---------- table will be created with firstparse --------------- // 

        struct table t;
        struct table *tPtr = &t;
        table_init(tPtr);
	int numLabels = 0; 
        numLabels = firstPass(instrPtr, tPtr);


//-------------------------------------------------------------------
// ----------- creating new array of instr string w/o label so each instr has real addresss --//
          // removing labels //
        char **realInstrPtr;
        realInstrPtr = calloc(numInstr - numLabels, 512 * sizeof(char));
        
        int realAddress = 0;
        for(int i = 0; i < numInstr; i++){
          char *instruction = instrPtr[i];
          if(strchr(instruction,':') == NULL){
            realInstrPtr[realAddress] = instrPtr[i]; 
            realAddress++ ;
          }
        }
//	assemble loop
	
	int i = 0;
	int outNum = 0;
	int unpassedOutnum = 0;
	int *outputNum = &outNum;
	uint32_t *outBuffer = calloc(numInstr*2, sizeof(uint32_t)/sizeof(char));
//	calloc max number of instr possible -> need to use some for sdthandler too
	
	int finalOutputNum = numInstr - numLabels;       //need to use (numInstr - numLabels)          
	int *fonPtr = &finalOutputNum;
	while (realInstrPtr[i] != NULL) {
	
	  char* instr;
	  instr = realInstrPtr[i];

	  char** tokens;			// unknown length of each token?
	  tokens = calloc(6, 512 * sizeof(char));  

	  token = strtok(instr, " ,");
	  count = 0;

	  while (token != NULL) {
	   tokens[count] = token;
	   token = strtok(NULL, " ,");
	   count ++;
	  }

	 uint32_t bin;
         bin = assemble(tokens, i, tPtr, &outBuffer, fonPtr, outputNum);		// call the switch fucntion to decide which handler
	 if (bin != 4294967295) { 		// ignore "label:"
	  outBuffer[unpassedOutnum] = bin;	  // write to output buffer with correct endianess
	  (outNum)++;
	  unpassedOutnum++;
	 }
	 
	 free(tokens);
	 i++;
        }

// end loop
//---------------------------------------------------------------------

// output compiled instructions from buffer

	FILE *out;
	if ((out = fopen(argv[2], "w")) == NULL) {
	 perror("Error opening output file!");
	 exit(EXIT_FAILURE);
	}
	fwrite(outBuffer, sizeof(uint32_t)/sizeof(char), outNum, out);

	fclose(out);


        table_destroy(tPtr);

	
        free(realInstrPtr);
        free(outBuffer);
	free(inputPtr);
	free(instrPtr);
  return EXIT_SUCCESS;
}

//---------------------------------------------------------------------

// assemble calls different handlers for different types of instr

uint32_t assemble(char** ts, int address, struct table *tPtr, uint32_t **outBuffer, int* finalOutputNum, int* outputNum) {

	if (!strcmp(ts[0], "and")
	 || !strcmp(ts[0], "eor")
	 || !strcmp(ts[0], "sub")
	 || !strcmp(ts[0], "rsb")
	 || !strcmp(ts[0], "add")
	 || !strcmp(ts[0], "orr")) {
	 return computeHandler(ts);
	}
	if (!strcmp(ts[0], "mov")) {
	 return movHandler(ts);
	}
	if (!strcmp(ts[0], "tst")
	 || !strcmp(ts[0], "teq")
	 || !strcmp(ts[0], "cmp")) {
	 return cpsrHandler(ts);
	}
	if (!strcmp(ts[0], "mul")) {
	 return mulHandler(ts);
	}
	if (!strcmp(ts[0], "mla")) {
	 return mlaHandler(ts);
	}
	if (!strcmp(ts[0], "ldr")
	 || !strcmp(ts[0], "str")) {
	 return sdtHandler(ts, address, outBuffer, finalOutputNum, outputNum);
	}
	if (ts[0][0] == 'b') {
	 return branchHandler(ts,tPtr, address);
	}
	if (!strcmp(ts[0], "lsl")) {
	 char *exp = ts[2];
	 ts[2] = ts[1];
//	 char lsl[4] = "lsl ";
//	 char newstr[4 + strlen(exp)];
//	 strcpy(newstr, lsl);
//	 strcat(newstr, exp);
	 ts[3] = "lsl";
	 ts[4] = exp;
	 return movHandler(ts);
	}
	if (!strcmp(ts[0], "andeq") && !strcmp(ts[1], "r0") && !strcmp(ts[2], "r0") && !strcmp(ts[3], "r0")) {
	 return 0;
	}
	int k = 0;
	while (ts[0][k] != '\0') {k++;}
	if (ts[0][k-1] == ':') {
	 return 4294967295;		// max value of uint32, special print case ignore
	}

        //return 0 when it reaches lable? instrPtr[10] = "loops:" ?

	//no, return 0 is only for compiling now, 
	//"loops:" should be ignored (case added, instr will be ignored during print)
	return 4294967295;		//return 0 to be replaced by error once we finish everything
}



