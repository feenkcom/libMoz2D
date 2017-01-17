/*
 * debug.h
 *
 *  Created on: Aug 21, 2016
 *      Author: Aliaksei Syrel
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#include "exports.h"

/* FOREGROUND */
#define RST  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define FRED(x) KRED << x << RST
#define FGRN(x) KGRN << x << RST
#define FYEL(x) KYEL << x << RST
#define FBLU(x) KBLU << x << RST
#define FMAG(x) KMAG << x << RST
#define FCYN(x) KCYN << x << RST
#define FWHT(x) KWHT << x << RST

#define BOLD(x) "\x1B[1m" << x << RST
#define UNDL(x) "\x1B[4m" << x << RST

#ifdef __cplusplus
extern "C" {
#endif

LIBRARY_API void moz2d_print_stderr (const char* message);
LIBRARY_API void moz2d_print_success (const char* message);
LIBRARY_API bool moz2d_test ();

#ifdef __cplusplus
}
#endif


#endif /* DEBUG_H_ */