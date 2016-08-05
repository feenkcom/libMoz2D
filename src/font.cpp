/*
 * font.cpp
 *
 *  Created on: Jul 26, 2016
 *      Author: Aliaksei Syrel
 */

#include "font.h"
#include <iostream>
#include <string>

#include "thebes/gfxPlatformFontList.h"

#include "nsGkAtoms.h"

//ScaledFont* moz2d_mac_font_create_scaled_font(const char* fontName, double size) {
//	CGFontRef mCGFont = moz2d_mac_font_get_cf_font_ref(fontName, size);
//	if (!mCGFont) {
//		std::cout << "CGFontRef is null" << "\n";
//		return nullptr;
//	}
//
//	NativeFont nativeFont;
//	nativeFont.mType = NativeFontType::MAC_FONT_FACE;
//	nativeFont.mFont = mCGFont;
//	ScaledFont* scaledFont = Factory::CreateScaledFontForNativeFont(nativeFont, size).take();
//	if (!scaledFont) {
//		std::cout << "scaledFont is null" << "\n";
//		return nullptr;
//	}
//	return scaledFont;
//}
//
//const char* moz2d_mac_font_create_scaled_font_data_name(const char* data, size_t size, double fontSize) {
//	CGFontRef mCGFont = moz2d_mac_font_get_cf_font_ref_data(data, size, fontSize);
//	const char* fontName = moz2d_mac_font_cg_font_full_name (mCGFont);
//	return fontName;
//}
//
//ScaledFont* moz2d_mac_font_create_scaled_font_data(const char* data, size_t size, double fontSize) {
//	CGFontRef mCGFont = moz2d_mac_font_get_cf_font_ref_data(data, size, fontSize);
//	if (!mCGFont) {
//		std::cout << "CGFontRef is null" << "\n";
//		return nullptr;
//	}
//
//	NativeFont nativeFont;
//	nativeFont.mType = NativeFontType::MAC_FONT_FACE;
//	nativeFont.mFont = mCGFont;
//
//	ScaledFont* scaledFont = Factory::CreateScaledFontForNativeFont(nativeFont, fontSize).take();
//	if (!scaledFont) {
//		std::cout << "scaledFont is null" << "\n";
//		return nullptr;
//	}
//	return scaledFont;
//}

void moz2d_pass_string_ascii(const char* string) {
	std::cout << "string-ascii: '" << string << "'\n";
}

void moz2d_pass_string_utf8(const char* string) {
	std::cout << "string-utf8: '" << string << "'\n";
}

void moz2d_pass_string_utf16(const char16_t* string) {
	std::cout << "string-utf16: '" << NS_LossyConvertUTF16toASCII(string).get() << "'\n";
	printf("%s\n", NS_ConvertUTF16toUTF8(string).get());
	std::cout << "string-utf16: '" << string << "'\n";
	std::cout << "string-utf16: '" << NS_ConvertUTF16toUTF8(string).get() << "'\n";
}

void moz2d_pass_wide_string(const wchar_t* string) {
	std::wcout << L"wide string-utf16: '" << string << L"'" << std::endl;
}

const char16_t* moz2d_convert_to_utf16_from_utf8(const char* string) {
	return NS_ConvertUTF8toUTF16(string).get();
}

const char16_t* moz2d_convert_to_utf16_from_ascii(const char* string) {
	return NS_ConvertASCIItoUTF16(string).get();
}

const char16_t* moz2d_rus_in_utf16() {
	return NS_LITERAL_STRING("абвг").get();
}

const char16_t* moz2d_eng_in_utf16() {
	return NS_LITERAL_STRING("abcd").get();
}

const char16_t* moz2d_emoji_in_utf16() {
	return NS_LITERAL_STRING("а🛅").get();
}

void moz2d_print_array(uint32_t* array, uint32_t length) {
	std::cout << "array: [";
	for (uint32_t i = 0; i < length; i++) {
		std::cout << array[i];
		if (i + 1 < length)
			std::cout << ", ";
	}
	std::cout << "]\n";
}
