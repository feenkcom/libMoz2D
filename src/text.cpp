/*
 * text.cpp
 *
 *  Created on: Jul 25, 2016
 *      Author: aliaksei
 */

#include "text.h"

const char16_t * moz2d_text_convert_ascii_to_utf16(const char* aCString, uint32_t aLength) {
	return NS_ConvertASCIItoUTF16(aCString, aLength).Data();
}
