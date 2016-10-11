/*
 * font_family_list.h
 *
 *  Created on: Aug 2, 2016
 *      Author: Aliaksei Syrel
 */

#ifndef FONT_FAMILY_LIST_H_
#define FONT_FAMILY_LIST_H_

#include "exports.h"
#include "gfxFontFamilyList.h"

using namespace mozilla;

#ifdef __cplusplus
extern "C" {
#endif


LIBRARY_API FontFamilyList* moz2d_font_family_list_create ();
/*
 * Create a font family list with one generic font defined by aType
 * My caller is responsible for freeing memory when family name is not needed anymore
 */
LIBRARY_API FontFamilyList* moz2d_font_family_list_create_generic (FontFamilyType aType);

/*
 * Create a font family list with one named font defined by aFamilyName
 * My caller is responsible for freeing memory when family name is not needed anymore
 */
LIBRARY_API FontFamilyList* moz2d_font_family_list_create_named_ascii (const char* aFamilyName, bool aQuoted);
LIBRARY_API FontFamilyList* moz2d_font_family_list_create_named_utf8 (const char* aFamilyName, bool aQuoted);

LIBRARY_API void moz2d_font_family_list_append_family_name(FontFamilyList* aFontFanilyList, FontFamilyName* aFamilyName);
LIBRARY_API void moz2d_font_family_list_append_family_named_ascii(FontFamilyList* aFontFamilyList, const char* aFamilyName);
LIBRARY_API void moz2d_font_family_list_append_family_named_utf8(FontFamilyList* aFontFamilyList, const char* aFamilyName);
LIBRARY_API void moz2d_font_family_list_append_family_generic(FontFamilyList* aFontFamilyList, FontFamilyType aType);
LIBRARY_API void moz2d_font_family_list_append_family_names(FontFamilyList* aFontFamilyList, FontFamilyName** aListOfFamilyNames, uint32_t length);
LIBRARY_API void moz2d_font_family_list_append_family_names_named_ascii(FontFamilyList* aFontFamilyList, const char** aListOfFamilyNames, uint32_t length);
LIBRARY_API void moz2d_font_family_list_append_family_names_named_utf8(FontFamilyList* aFontFamilyList, const char** aListOfFamilyNames, uint32_t length);
LIBRARY_API void moz2d_font_family_list_append_family_names_generic(FontFamilyList* aFontFamilyList, FontFamilyType* aListOfFamilyTypes, uint32_t length);

LIBRARY_API void moz2d_font_family_list_clear(FontFamilyList* aFontFamilyList);
LIBRARY_API uint32_t moz2d_font_family_list_length(FontFamilyList* aFontFamilyList);
LIBRARY_API FontFamilyType moz2d_font_family_list_get_default_font_type(FontFamilyList* aFontFamilyList);
LIBRARY_API void moz2d_font_family_list_set_default_font_type(FontFamilyList* aFontFamilyList, FontFamilyType aFontType);
LIBRARY_API const char* moz2d_font_family_list_to_string(FontFamilyList* aFontFamilyList, bool aQuotes, bool aIncludeDefault);
/*
 * Return a copied list of family names.
 * Caller is responsible for freeing memory.
 * Amount of font family names will be stored in length
 */
LIBRARY_API FontFamilyName** moz2d_font_family_list_get_font_list(FontFamilyList* aFontFamilyList, uint32_t* length);

#ifdef __cplusplus
}
#endif

#endif /* FONT_FAMILY_LIST_H_ */
