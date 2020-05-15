#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "linux_parser.h"

class Processor {
 public:
  float Utilization();

 private:
  long prevtotal{0};
  long previdle{0};
};

#endif