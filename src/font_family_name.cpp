/*
 * font_family_name.cpp
 *
 *  Created on: Aug 2, 2016
 *      Author: Aliaksei Syrel
 */

#include "font_family_name.h"
#include "nsString.h"

FontFamilyName* moz2d_font_family_name_create_generic(FontFamilyType aType) {
	return new FontFamilyName(aType);
}

FontFamilyName* moz2d_font_family_name_create_named_ascii(const char* aFamilyName, bool aQuoted) {
	return new FontFamilyName(NS_ConvertASCIItoUTF16(aFamilyName), aQuoted ? eQuotedName : eUnquotedName);
}

FontFamilyName* moz2d_font_family_name_create_named_utf8(const char* aFamilyName, bool aQuoted) {
	return new FontFamilyName(NS_ConvertUTF8toUTF16(aFamilyName),  aQuoted ? eQuotedName : eUnquotedName);
}

bool moz2d_font_family_name_is_named (FontFamilyName* aFontFamilyName) {
	return aFontFamilyName->IsNamed();
}

bool moz2d_font_family_name_is_generic (FontFamilyName* aFontFamilyName) {
	return aFontFamilyName->IsGeneric();
}

FontFamilyType moz2d_font_family_name_get_type (FontFamilyName* aFontFamilyName) {
	return aFontFamilyName->mType;
}

const char* moz2d_font_family_name_get_name (FontFamilyName* aFontFamilyName) {
	return ToNewUTF8String(aFontFamilyName->mName);
}

const char* moz2d_font_family_name_to_string (FontFamilyName* aFontFamilyName, bool aQuotes) {
	nsAdoptingString string;
	aFontFamilyName->AppendToString(string, aQuotes);
	return ToNewUTF8String(string);
}
