/*
 * general.cc
 *
 *  Created on: Nov 27, 2010
 *      Author: tats
 */

#include "general.h"

void error(const char* msg) {
  // Iff not arduino.
  std::cout << msg << std::endl;
  exit(-1);
}
