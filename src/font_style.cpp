/*
 * font_style.cpp
 *
 *  Created on: Aug 1, 2016
 *      Author: Aliaksei Syrel
 */

#include "font_style.h"

#include "nsString.h"
#include "thebes/gfxPlatformFontList.h"
#include "nsGkAtoms.h"

struct gfxPlatformFontListExposer : gfxPlatformFontList {
  using gfxPlatformFontList::TryLangForGroup;
  using gfxPlatformFontList::GetLangService;
};

nsIAtom* private_moz2d_language_to_atom (const char* anOSLang) {
	nsILanguageAtomService* langService = (static_cast<gfxPlatformFontListExposer*>(gfxPlatformFontList::PlatformFontList())->*&gfxPlatformFontListExposer::GetLangService)();

	nsCString aOSLang = nsCString(anOSLang);
	nsIAtom* langAtom = nsGkAtoms::x_western;
	nsCString aFcLang = nsCString("");

	(static_cast<gfxPlatformFontListExposer*>(gfxPlatformFontList::PlatformFontList())->*&gfxPlatformFontListExposer::TryLangForGroup)(aOSLang, langAtom,aFcLang);
	nsIAtom *atom = langService->LookupLanguage(aFcLang);

	if (!atom) atom = langAtom;
	return atom;
}

gfxFontStyle* moz2d_font_style_create (
		uint8_t aStyle, // normal, italic, bold
		uint16_t aWeight,
		int16_t aStretch,
        float aSize,
        const char* aLanguage,
        bool aExplicitLanguage,
        float aSizeAdjust,
        bool aSystemFont,
        bool aPrinterFont,
        bool aWeightSynthesis,
        bool aStyleSynthesis,
        const char* aLanguageOverride) {
	return new gfxFontStyle(
			aStyle,
			aWeight,
			aStretch,
			aSize,
			private_moz2d_language_to_atom(aLanguage),
			aExplicitLanguage,
			aSizeAdjust,
			aSystemFont,
			aPrinterFont,
			aWeightSynthesis,
			aStyleSynthesis,
			NS_ConvertASCIItoUTF16(aLanguageOverride));
}

gfxFontStyle* moz2d_font_style_create_default () {
	return new gfxFontStyle();
}

uint8_t moz2d_font_style_get_style(gfxFontStyle* aFontStyle) {
	return aFontStyle->style;
}

uint16_t moz2d_font_style_get_weight(gfxFontStyle* aFontStyle) {
	return aFontStyle->weight;
}

uint16_t moz2d_font_style_get_stretch(gfxFontStyle* aFontStyle){
	return aFontStyle->stretch;
}

float moz2d_font_style_get_size(gfxFontStyle* aFontStyle){
	return aFontStyle->size;
}

void moz2d_font_style_set_size(gfxFontStyle* aFontStyle, float size) {
	aFontStyle->size = size;
}

const char * moz2d_font_style_get_language(gfxFontStyle* aFontStyle) {
	RefPtr<nsIAtom> language = aFontStyle->language;
	if (!language)
		return NS_LITERAL_CSTRING("").get();
	nsAdoptingCString name;
	language->ToUTF8String(name);
	return name.get();
}

void moz2d_font_style_set_language(gfxFontStyle* aFontStyle, const char* anOSLang) {
	aFontStyle->language = private_moz2d_language_to_atom(anOSLang);
}

bool moz2d_font_style_get_is_explicit_language(gfxFontStyle* aFontStyle) {
	return aFontStyle->explicitLanguage;
}

float moz2d_font_style_get_size_adjust(gfxFontStyle* aFontStyle) {
	return aFontStyle->sizeAdjust;
}

bool moz2d_font_style_get_is_system_font(gfxFontStyle* aFontStyle) {
	return aFontStyle->systemFont;
}

bool moz2d_font_style_get_is_printer_font(gfxFontStyle* aFontStyle) {
	return aFontStyle->printerFont;
}

bool moz2d_font_style_get_is_weight_synthesis(gfxFontStyle* aFontStyle) {
	return aFontStyle->allowSyntheticWeight;
}

bool moz2d_font_style_get_is_style_synthesis(gfxFontStyle* aFontStyle) {
	return aFontStyle->allowSyntheticStyle;
}

uint32_t moz2d_font_style_get_override_language(gfxFontStyle* aFontStyle) {
	return aFontStyle->languageOverride;
}
