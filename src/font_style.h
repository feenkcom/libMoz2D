/*
 * font_style.h
 *
 *  Created on: Aug 1, 2016
 *      Author: Aliaksei Syrel
 */

#ifndef FONT_STYLE_H_
#define FONT_STYLE_H_

#include "exports.h"
#include "gfxFont.h"

using namespace mozilla;

#ifdef __cplusplus
extern "C" {
#endif

/* Create a font style from provided properties
 * aLanguage must be in ASCII format
 * aLanguageOverride must be in ASCII format
 */
LIBRARY_API gfxFontStyle* moz2d_font_style_create (
		uint8_t aStyle, // normal, italic, bold
		uint16_t aWeight,
		int16_t aStretch,
		float aSize,
		const char *aLanguage, // ASCII
		bool aExplicitLanguage,
		float aSizeAdjust,
		bool aSystemFont,
		bool aPrinterFont,
		bool aWeightSynthesis,
		bool aStyleSynthesis,
		const char* aLanguageOverride // ASCII
);

/* Create a default font style
 */
LIBRARY_API gfxFontStyle* moz2d_font_style_create_default ();

// Various accessors

LIBRARY_API uint8_t moz2d_font_style_get_style(gfxFontStyle* aFontStyle);
LIBRARY_API uint16_t moz2d_font_style_get_weight(gfxFontStyle* aFontStyle);
LIBRARY_API uint16_t moz2d_font_style_get_stretch(gfxFontStyle* aFontStyle);
LIBRARY_API float moz2d_font_style_get_size(gfxFontStyle* aFontStyle);
LIBRARY_API void moz2d_font_style_set_size(gfxFontStyle* aFontStyle, float size);
LIBRARY_API const char * moz2d_font_style_get_language(gfxFontStyle* aFontStyle);
LIBRARY_API void moz2d_font_style_set_language(gfxFontStyle* aFontStyle, const char * language);
LIBRARY_API bool moz2d_font_style_get_is_explicit_language(gfxFontStyle* aFontStyle);
LIBRARY_API float moz2d_font_style_get_size_adjust(gfxFontStyle* aFontStyle);
LIBRARY_API bool moz2d_font_style_get_is_system_font(gfxFontStyle* aFontStyle);
LIBRARY_API bool moz2d_font_style_get_is_printer_font(gfxFontStyle* aFontStyle);
LIBRARY_API bool moz2d_font_style_get_is_weight_synthesis(gfxFontStyle* aFontStyle);
LIBRARY_API bool moz2d_font_style_get_is_style_synthesis(gfxFontStyle* aFontStyle);
LIBRARY_API uint32_t moz2d_font_style_get_override_language(gfxFontStyle* aFontStyle);
LIBRARY_API void moz2d_font_style_delete(gfxFontStyle* aFontStyle);

#ifdef __cplusplus
}
#endif


#endif /* FONT_STYLE_H_ */
