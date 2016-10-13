/*
 * preferences.cpp
 *
 *  Created on: Oct 12, 2016
 *      Author: Aliaksei Syrel
 */

#include "preferences.h"
#include "mozilla/Preferences.h"

using namespace mozilla;

void moz_preferences_set_bool(const char* aPref, bool aValue) {
	Preferences::SetBool(aPref, aValue);
}

void moz_preferences_set_float(const char* aPref, float aValue) {
	Preferences::SetFloat(aPref, aValue);
}

void moz_preferences_set_int(const char* aPref, int32_t aValue) {
	Preferences::SetInt(aPref, aValue);
}

void moz_preferences_set_string(const char* aPref, const char* aValue) {
	Preferences::SetCString(aPref, aValue);
}