#import <AppKit/AppKit.h>
#import <Foundation/Foundation.h>

#include "nsCharTraits.h"
#include "mac_font.h"
#include <iostream>
#include <string>

CGFontRef Moz2DGetFontRef(NSString *fontName, CGFloat sizeHint)
{
	NSLog(@"Your font is %@ of size %f", fontName, sizeHint);
	CGFontRef fontRef = ::CGFontCreateWithFontName(CFStringRef(fontName));
	if (!fontRef) {
		// This happens on macOS 10.12 for font entry names that start with
		// .AppleSystemUIFont. For those fonts, we need to go through NSFont
		// to get the correct CGFontRef.
		// Both the Text and the Display variant of the display font use
		// .AppleSystemUIFontSomethingSomething as their member names.
		// That's why we're carrying along mSizeHint to this place so that
		// we get the variant that we want for this family.
		NSFont* font = [NSFont fontWithName:fontName size:sizeHint];
		if (font) {
			fontRef = CTFontCopyGraphicsFont((CTFontRef)font, nullptr);
		}
	}
	return fontRef;
}

CGFontRef Moz2DGetFontRefData(NSData *inData, CGFloat sizeHint) {
	CFErrorRef error;
	CGDataProviderRef provider = CGDataProviderCreateWithCFData((CFDataRef)inData);
	CGFontRef font = CGFontCreateWithDataProvider(provider);
	if (! CTFontManagerRegisterGraphicsFont(font, &error)) {
	    CFStringRef errorDescription = CFErrorCopyDescription(error);
	    NSLog(@"Failed to load font: %@", errorDescription);
	    CFRelease(errorDescription);
		return NULL;
	}
	CFStringRef tmpFontName = CGFontCopyFullName ( font );
	NSString *fontName = (NSString *)tmpFontName;
	
	CFRelease(provider);
	//CFRelease(font);
	
	//return Moz2DGetFontRef(fontName, sizeHint);
	return font;
}

char * MYCFStringCopyUTF8String(CFStringRef aString) {
  if (aString == NULL) {
    return NULL;
  }

  CFIndex length = CFStringGetLength(aString);
  CFIndex maxSize =
  CFStringGetMaximumSizeForEncoding(length, kCFStringEncodingUTF8) + 1;
  char *buffer = (char *)malloc(maxSize);
  if (CFStringGetCString(aString, buffer, maxSize,
                         kCFStringEncodingUTF8)) {
    return buffer;
  }
  return NULL;
}

CGFontRef moz2d_mac_font_get_cf_font_ref(const char* fontName, double size) {
	std::cout << fontName << "\n";
	
	return Moz2DGetFontRef([NSString stringWithUTF8String:fontName], size);
}

CGFontRef moz2d_mac_font_get_cf_font_ref_data(const char* data, size_t size, double fontSize) {
	NSData* inData = [NSData dataWithBytes:(const void *)data length:sizeof(char)*size];
	return Moz2DGetFontRefData(inData, fontSize);
}

void CGContextSetFontSize ( CGContextRef c, double size );

const char* moz2d_mac_font_cg_font_full_name (CGFontRef fontRef) {
	return MYCFStringCopyUTF8String(CGFontCopyFullName ( fontRef ) );
}


