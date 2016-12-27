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

bool moz2d_text_convert_char32_to_char16(const uint32_t* aCString, uint32_t aLength, char16_t* aBuffer) {
    for (uint32_t i = 0; i < aLength; i++) {
        if (aCString[i] > std::numeric_limits<char16_t>::max())
            return false;
        aBuffer[i] = (char16_t)aCString[i];
    }
    return true;
}