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

void moz2d_print_success (const char* message) {
	std::cout << BOLD(FGRN(message)) << std::endl;
}

bool moz2d_test () {
    return true;
}