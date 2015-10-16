#ifndef STATE_H
#define STATE_H
#include <stdint.h>
struct state {
  uint32_t reg[17];
  //reg 13, 14 are ignored.  [15 is PC, 16 is CPSR]
};

#endif
