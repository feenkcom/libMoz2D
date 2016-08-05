/*
 * font_family_list.cpp
 *
 *  Created on: Aug 2, 2016
 *      Author: Aliaksei Syrel
 */

#include "font_family_list.h"
#include "nsString.h"


FontFamilyList* moz2d_font_family_list_create () {
	return new FontFamilyList();
}

FontFamilyList* moz2d_font_family_list_create_generic (FontFamilyType aType) {
	return new FontFamilyList(aType);
}

FontFamilyList* moz2d_font_family_list_create_named_ascii (const char* aFamilyName, bool aQuoted) {
	return new FontFamilyList(NS_ConvertASCIItoUTF16(aFamilyName), aQuoted ? eQuotedName : eUnquotedName);
}

FontFamilyList* moz2d_font_family_list_create_named_utf8 (const char* aFamilyName, bool aQuoted) {
	return new FontFamilyList(NS_ConvertUTF8toUTF16(aFamilyName), aQuoted ? eQuotedName : eUnquotedName);
}

void moz2d_font_family_list_append_family_name(FontFamilyList* aFontFamilyList, FontFamilyName* aFamilyName) {
	aFontFamilyList->Append(FontFamilyName(*aFamilyName));
}


void moz2d_font_family_list_append_family_named_ascii(FontFamilyList* aFontFamilyList, const char* aFamilyName) {
	aFontFamilyList->Append(FontFamilyName(NS_ConvertASCIItoUTF16(aFamilyName)));
}

void moz2d_font_family_list_append_family_named_utf8(FontFamilyList* aFontFamilyList, const char* aFamilyName) {
	aFontFamilyList->Append(FontFamilyName(NS_ConvertUTF8toUTF16(aFamilyName)));
}

void moz2d_font_family_list_append_family_generic(FontFamilyList* aFontFamilyList, FontFamilyType aType) {
	aFontFamilyList->Append(FontFamilyName(aType));
}

void moz2d_font_family_list_append_family_names(FontFamilyList* aFontFamilyList, FontFamilyName** aListOfFamilyNames, uint32_t length) {
	for (uint32_t i = 0; i < length; i++) {
		aFontFamilyList->Append(FontFamilyName(*aListOfFamilyNames[i]));
	}
}

void moz2d_font_family_list_append_family_names_named_ascii(FontFamilyList* aFontFamilyList, const char** aListOfFamilyNames, uint32_t length) {
	for (uint32_t i = 0; i < length; i++) {
		moz2d_font_family_list_append_family_named_ascii(aFontFamilyList, aListOfFamilyNames[i]);
	}
}

void moz2d_font_family_list_append_family_names_named_utf8(FontFamilyList* aFontFamilyList, const char** aListOfFamilyNames, uint32_t length) {
	for (uint32_t i = 0; i < length; i++) {
		moz2d_font_family_list_append_family_named_utf8(aFontFamilyList, aListOfFamilyNames[i]);
	}
}

void moz2d_font_family_list_append_family_names_generic(FontFamilyList* aFontFamilyList, FontFamilyType* aListOfFamilyTypes, uint32_t length) {
	for (uint32_t i = 0; i < length; i++) {
		moz2d_font_family_list_append_family_generic(aFontFamilyList, aListOfFamilyTypes[i]);
	}
}

void moz2d_font_family_list_clear(FontFamilyList* aFontFamilyList) {
	aFontFamilyList->Clear();
}

uint32_t moz2d_font_family_list_length(FontFamilyList* aFontFamilyList) {
	return aFontFamilyList->Length();
}

FontFamilyType moz2d_font_family_list_get_default_font_type(FontFamilyList* aFontFamilyList) {
	return aFontFamilyList->GetDefaultFontType();
}

void moz2d_font_family_list_set_default_font_type(FontFamilyList* aFontFamilyList, FontFamilyType aFontType) {
	aFontFamilyList->SetDefaultFontType(aFontType);
}

const char* moz2d_font_family_list_to_string(FontFamilyList* aFontFamilyList, bool aQuotes, bool aIncludeDefault) {
	nsAdoptingString string;
	aFontFamilyList->ToString(string, aQuotes,aIncludeDefault);
	return ToNewUTF8String(string);
}

FontFamilyName**  moz2d_font_family_list_get_font_list(FontFamilyList* aFontFamilyList, uint32_t* retLength) {
	uint32_t length = aFontFamilyList->Length();

	FontFamilyName **names = static_cast<FontFamilyName **>(malloc(length * sizeof(FontFamilyName *)));
	FontFamilyName * namePointer;

	const nsTArray<FontFamilyName>& list = aFontFamilyList->GetFontlist();
	for (uint32_t i = 0; i < length; i++) {
		namePointer = new FontFamilyName(list.ElementAt(i));
		names[i] = namePointer;
	}

	*retLength = length;
	return names;
}

