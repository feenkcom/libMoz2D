/*
 * preferences.h
 *
 *  Created on: Oct 12, 2016
 *      Author: Aliaksei Syrel
 */

#ifndef PREFERENCES_H_
#define PREFERENCES_H_

#include "exports.h"

#ifdef __cplusplus
extern "C" {
#endif

	LIBRARY_API void moz_preferences_set_bool(const char* aPref, bool aValue);
	LIBRARY_API void moz_preferences_set_float(const char* aPref, float aValue);
	LIBRARY_API void moz_preferences_set_int(const char* aPref, int32_t aValue);
	LIBRARY_API void moz_preferences_set_string(const char* aPref, const char* aValue);

#ifdef __cplusplus
}
#endif

#endif /* PREFERENCES_H_ */
