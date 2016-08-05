/*
 * font_family_name.h
 *
 *  Created on: Aug 2, 2016
 *      Author: Aliaksei Syrel
 */

#ifndef FONT_FAMILY_NAME_H_
#define FONT_FAMILY_NAME_H_

#include "gfxFontFamilyList.h"

using namespace mozilla;

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Create generic font family - e.g. sans-serif
 * My caller is responsible for freeing memory when family name is not needed anymore
 */
FontFamilyName* moz2d_font_family_name_create_generic (FontFamilyType aType);

/*
 * Create named font family - e.g. Helvetica.
 * A name must be represented in ASCII format
 * My caller is responsible for freeing memory when family name is not needed anymore
 */
FontFamilyName* moz2d_font_family_name_create_named_ascii (const char* aFamilyName, bool aQuoted);

/*
 * Create named font family - e.g. Helvetica.
 * A name must be represented in UTF8 format
 * My caller is responsible for freeing memory when family name is not needed anymore
 */
FontFamilyName* moz2d_font_family_name_create_named_utf8 (const char* aFamilyName, bool aQuoted);

bool moz2d_font_family_name_is_named (FontFamilyName* aFontFamilyName);
bool moz2d_font_family_name_is_generic (FontFamilyName* aFontFamilyName);
FontFamilyType moz2d_font_family_name_get_type (FontFamilyName* aFontFamilyName);
const char* moz2d_font_family_name_get_name (FontFamilyName* aFontFamilyName);
const char* moz2d_font_family_name_to_string (FontFamilyName* aFontFamilyName, bool aQuotes);

#ifdef __cplusplus
}
#endif

#endif /* FONT_FAMILY_NAME_H_ */
