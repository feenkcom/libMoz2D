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

ScaledFont* moz2d_text_create_scaled_font(NativeFont* nativeFont, Float aSize) {
	return Factory::CreateScaledFontForNativeFont(*nativeFont, aSize).take();
}

ScaledFont* moz2d_text_create_scaled_font_from_cairo(cairo_scaled_font_t* cairoScaledFont, Float aSize) {
	NativeFont nativeFont;
	nativeFont.mType = NativeFontType::CAIRO_FONT_FACE;
	nativeFont.mFont = cairoScaledFont;
	return Factory::CreateScaledFontForNativeFont(nativeFont, aSize).take();
}

cairo_font_face_t * moz2d_text_cairo_ft_font_face_create_for_ft_face(FT_Face aFace , int flags ) {
	return cairo_ft_font_face_create_for_ft_face(aFace, flags);
}

cairo_scaled_font_t * moz2d_text_cairo_scaled_font_create (cairo_font_face_t *font_face,
                          const cairo_matrix_t *font_matrix,
                          const cairo_matrix_t *ctm,
                          const cairo_font_options_t *options) {
	return cairo_scaled_font_create (font_face,
            font_matrix,
            ctm,
            options);
}

FontType moz2d_text_fill_glyphs_cairo (DrawTarget* drawTarget, ScaledFont *aFont, CairoGlyph* cairoGlyphs, uint32_t numGlyphs, Pattern* aPattern, DrawOptions *aOptions) {
	GlyphBuffer buffer;
	buffer.mNumGlyphs = numGlyphs;
	Glyph* glyphs = new Glyph[numGlyphs];
	for (unsigned int i = 0; i < numGlyphs; i++) {
		glyphs[i].mIndex = cairoGlyphs[i].index;
		glyphs[i].mPosition = Point(cairoGlyphs[i].x, cairoGlyphs[i].y);
	}
	buffer.mGlyphs = glyphs;
//
//	Glyph glyphs[1] = { { 9, Point(100,100) } };
//	GlyphBuffer buffer = { glyphs , 1 };

	drawTarget->FillGlyphs(aFont, buffer, *aPattern, *aOptions);


	buffer.mGlyphs = nullptr;
	delete[] glyphs;
	return aFont->GetType();
}


