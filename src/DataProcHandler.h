#ifndef DATAPROCHANDLER_H
#define DATAPROCHANDLER_H

uint32_t rotateRTwo(uint32_t exp);
uint32_t operandHandler(char *operand, char *shift, char *reg);
uint32_t computeHandler(char **tokens);
uint32_t movHandler(char **tokens);
uint32_t cpsrHandler(char **tokens);


#endif
