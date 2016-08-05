/*
 * font.h
 *
 *  Created on: Jul 26, 2016
 *      Author: Aliaksei Syrel
 */

#ifndef FONT_H_
#define FONT_H_

#include "2d/2D.h"

#include "gfxTextRun.h"
#include "gfxFontFamilyList.h"


using namespace mozilla;
using namespace mozilla::gfx;

#ifdef __cplusplus
extern "C" {
#endif

void moz2d_pass_string_ascii(const char* string);

void moz2d_pass_string_utf8(const char* string);

void moz2d_pass_string_utf16(const char16_t* string);

void moz2d_pass_wide_string(const wchar_t* string);

const char16_t* moz2d_convert_to_utf16_from_utf8(const char* string);
const char16_t* moz2d_convert_to_utf16_from_ascii(const char* string);
const char16_t* moz2d_rus_in_utf16();
const char16_t* moz2d_eng_in_utf16();
const char16_t* moz2d_emoji_in_utf16();
void moz2d_print_array(uint32_t* array, uint32_t length);

#ifdef __cplusplus
}
#endif

#endif /* FONT_H_ */
