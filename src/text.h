/*
 * text.h
 *
 *  Created on: Jul 25, 2016
 *      Author: aliaksei
 */

#ifndef TEXT_H_
#define TEXT_H_
#define MOZILLA_INTERNAL_API

#include "2d/2D.h"
#include "nsString.h"
#include "cairo.h"
#include "cairo-ft.h"

using namespace mozilla::gfx;

struct CairoGlyph {
	uint32_t        index;
	double       x;
	double       y;
};

#ifdef __cplusplus
extern "C" {
#endif

const char16_t * moz2d_text_convert_ascii_to_utf16(const char* aCString, uint32_t aLength);
ScaledFont* moz2d_text_create_scaled_font(NativeFont* nativeFont, Float aSize);
ScaledFont* moz2d_text_create_scaled_font_from_cairo(cairo_scaled_font_t* cairoScaledFont, Float aSize);
cairo_font_face_t * moz2d_text_cairo_ft_font_face_create_for_ft_face(FT_Face aFace , int flags );
cairo_scaled_font_t * moz2d_text_cairo_scaled_font_create (cairo_font_face_t *font_face,
                          const cairo_matrix_t *font_matrix,
                          const cairo_matrix_t *ctm,
                          const cairo_font_options_t *options);

FontType moz2d_text_fill_glyphs_cairo (DrawTarget* drawTarget, ScaledFont *aFont, CairoGlyph* cairoGlyphs, uint32_t numGlyphs, Pattern* aPattern, DrawOptions *aOptions);

#ifdef __cplusplus
}
#endif

#endif /* TEXT_H_ */
