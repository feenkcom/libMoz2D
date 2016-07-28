/*
 * font.cpp
 *
 *  Created on: Jul 26, 2016
 *      Author: aliaksei
 */

#include "font.h"
#include <iostream>
#include <string>

ScaledFont* moz2d_mac_font_create_scaled_font(const char* fontName, double size) {
	CGFontRef mCGFont = moz2d_mac_font_get_cf_font_ref(fontName, size);
	if (!mCGFont) {
		std::cout << "CGFontRef is null" << "\n";
		return nullptr;
	}

	NativeFont nativeFont;
	nativeFont.mType = NativeFontType::MAC_FONT_FACE;
	nativeFont.mFont = mCGFont;
	ScaledFont* scaledFont = Factory::CreateScaledFontForNativeFont(nativeFont, size).take();
	if (!scaledFont) {
		std::cout << "scaledFont is null" << "\n";
		return nullptr;
	}
	return scaledFont;
}

const char* moz2d_mac_font_create_scaled_font_data_name(const char* data, size_t size, double fontSize) {
	CGFontRef mCGFont = moz2d_mac_font_get_cf_font_ref_data(data, size, fontSize);
	const char* fontName = moz2d_mac_font_cg_font_full_name (mCGFont);
	return fontName;
}

ScaledFont* moz2d_mac_font_create_scaled_font_data(const char* data, size_t size, double fontSize) {
	CGFontRef mCGFont = moz2d_mac_font_get_cf_font_ref_data(data, size, fontSize);
	if (!mCGFont) {
		std::cout << "CGFontRef is null" << "\n";
		return nullptr;
	}

	NativeFont nativeFont;
	nativeFont.mType = NativeFontType::MAC_FONT_FACE;
	nativeFont.mFont = mCGFont;

	ScaledFont* scaledFont = Factory::CreateScaledFontForNativeFont(nativeFont, fontSize).take();
	if (!scaledFont) {
		std::cout << "scaledFont is null" << "\n";
		return nullptr;
	}
	return scaledFont;
}
