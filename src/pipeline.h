#ifndef PIPELINE_H
#define PIPELINE_H
#include <stdint.h>
struct pipeline {
  uint32_t decoded;
  uint32_t fetched;
  int op;
};

#endif
