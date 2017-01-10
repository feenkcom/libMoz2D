/*
 * font_group.cpp
 *
 *  Created on: Aug 2, 2016
 *      Author: Aliaksei Syrel
 */

#include "font_group.h"
#include "nsString.h"
#include "gfxFont.h"

gfxFontGroup* moz2d_font_group_create (
		FontFamilyList* aFontFamilyList,
		gfxFontStyle* aStyle,
		gfxTextPerfMetrics* aTextPerf,
		gfxUserFontSet *aUserFontSet,
		float aDevToCssSize) {
	gfxFontGroup* fontGroup = gfxPlatform::GetPlatform()->CreateFontGroup(*aFontFamilyList, aStyle, aTextPerf, aUserFontSet, aDevToCssSize);
	// Need to increase reference, because my caller gets ownership over font group
	fontGroup->AddRef();
	return fontGroup;
}

void moz2d_font_group_metrics (gfxFontGroup* aFontGroup, gfxFont::Metrics* aMetrics, bool isVertical) {
	*aMetrics = aFontGroup->GetFirstValidFont()->GetMetrics(isVertical ? gfxFont::Orientation::eVertical : gfxFont::Orientation::eHorizontal);
	aMetrics->underlineOffset = aFontGroup->GetUnderlineOffset();
}

gfxTextRun* moz2d_font_group_make_text_run_ascii (
		DrawTarget* drawTarget,
		gfxFontGroup* aFontGroup,
		const uint8_t* aText,
		uint32_t aLength,
		uint32_t* initialBreaks,
		uint32_t initialBreakCount,
		int32_t aAppUnitsPerDevUnit,
		uint32_t aTextRunFactoryFlags) {

	gfxTextRunFactory::Parameters params = {
			drawTarget, nullptr, nullptr, initialBreaks, initialBreakCount, aAppUnitsPerDevUnit
	};

	return (aFontGroup->MakeTextRun (
			aText,
			aLength,
			&params,
			aTextRunFactoryFlags,
			nullptr)).take();
}

gfxTextRun* moz2d_font_group_make_text_run_utf16 (
        DrawTarget* drawTarget,
        gfxFontGroup* aFontGroup,
        const char16_t* aText,
        uint32_t aLength,
        uint32_t* initialBreaks,
        uint32_t initialBreakCount,
        int32_t aAppUnitsPerDevUnit,
        uint32_t aTextRunFactoryFlags) {

	gfxTextRunFactory::Parameters params = {
			drawTarget, nullptr, nullptr, initialBreaks, initialBreakCount, aAppUnitsPerDevUnit
	};

	return (aFontGroup->MakeTextRun (
			aText,
			aLength,
			&params,
			aTextRunFactoryFlags,
			nullptr)).take();
}