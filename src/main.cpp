/*
 * main.cpp
 *
 *  Created on: Jul 25, 2016
 *      Author: aliaksei
 */

#include <iostream>
#include <string>

#include "font.h"
#include "font_style.h"

#include "gfxContext.h"
#include "nsString.h"
#include "gfxTextRun.h"
#include "gfxFontEntry.h"
#include "gfxFont.h"
#include "thebes/gfxPlatformFontList.h"
#include "gfxPlatform.h"
#include "gfxPrefs.h"
#include "nsError.h"
#include "mozilla/Preferences.h"
//#include "mac_font.h"
#include "nsGkAtoms.h"
#include "services/services.h"
#include "PLDHashTable.h"

#include "font_family_list.h"
#include "font_group.h"
#include "font_style.h"
#include "draw_target.h"

void NS_ABORT_OOM(unsigned long error){

}

EXPORT_XPCOM_API(void)
NS_DebugBreak(uint32_t aSeverity, const char* aStr, const char* aExpr,
		const char* aFile, int32_t aLine)
		{

		}


int main () {
	moz2d_services_init();
	DrawTarget* drawTarget = moz2d_draw_target_create(BackendType::COREGRAPHICS, 300, 350, SurfaceFormat::B8G8R8A8);

	gfxFontStyle* style = moz2d_font_style_create_default();
	FontFamilyList* list = moz2d_font_family_list_create_named_ascii("Apple Color Emoji", false);
	gfxFontGroup* fontGroup = moz2d_font_group_create(list, style, nullptr, nullptr, 1.0);
	std::cout << "------------- text run ------------" << "\n";
	gfxTextRun* textRun = moz2d_font_group_make_text_run_utf16(
			drawTarget,
			fontGroup,
			NS_LITERAL_STRING("�").get(),
			1,
			nullptr,
			0,
			1,
			0);
	std::cout << "------------- text run metrics -------------------" << "\n";
	gfxTextRun::Metrics textRunMetrics = textRun->MeasureText(gfxFont::TIGHT_INK_EXTENTS, drawTarget);

	textRunMetrics.mBoundingBox.Scale(1.0 / 1.0);
	gfxRect mBoundingBox = textRunMetrics.mBoundingBox;
	gfxFloat advanceWidth = textRunMetrics.mAdvanceWidth;

	std::cout << "length: " << textRun->GetLength() << "\n";
	std::cout << "missing glyphs: " << textRun->CountMissingGlyphs() << "\n";
	std::cout << "boundingBox: (" << mBoundingBox.x << "@" << mBoundingBox.y << ") extent: (" << mBoundingBox.width << "@" << mBoundingBox.height << ")" << "\n";
	std::cout << "advanceWidth: " << advanceWidth << "\n";

	moz2d_services_shutdown();

	return 0;
}
