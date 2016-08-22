/*
 * debug.cpp
 *
 *  Created on: Aug 21, 2016
 *      Author: Aliaksei Syrel
 */

#include "debug.h"

#include <iostream>

void moz2d_print_stderr (const char* message) {
	std::cerr << BOLD(FRED(message)) << std::endl;
}
