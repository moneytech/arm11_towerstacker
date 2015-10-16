#ifndef SDT_HANDLER_H
#define SDT_HANDLER_H

#include "util.h"
#include "table.h"

uint32_t sdtHandler(char **ts, int address, uint32_t **outBuffer, int *finalOutputNum, int *outputNum);
  

#endif
