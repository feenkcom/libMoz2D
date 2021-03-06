/*
 * text.h
 *
 *  Created on: Jul 25, 2016
 *      Author: aliaksei
 */

#ifndef TEXT_H_
#define TEXT_H_

#include "exports.h"
#include "2d/2D.h"
#include "nsString.h"

using namespace mozilla::gfx;

#ifdef __cplusplus
extern "C" {
#endif

LIBRARY_API const char16_t * moz2d_text_convert_ascii_to_utf16(const char* aCString, uint32_t aLength);
LIBRARY_API bool moz2d_text_convert_char32_to_char16(const uint32_t* aCString, uint32_t aLength, char16_t* aBuffer);

#ifdef __cplusplus
}
#endif

#endif /* TEXT_H_ */
