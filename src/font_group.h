/*
 * font_group.h
 *
 *  Created on: Aug 2, 2016
 *      Author: Aliaksei Syrel
 */

#ifndef FONT_GROUP_H_
#define FONT_GROUP_H_

#include "exports.h"
#include "2d/2D.h"
#include "gfxTextRun.h"
#include "gfxFont.h"
#include "gfxFontFamilyList.h"

using namespace mozilla;
using namespace mozilla::gfx;

#ifdef __cplusplus
extern "C" {
#endif

LIBRARY_API gfxFontGroup* moz2d_font_group_create (
		FontFamilyList* aFontFamilyList,
		gfxFontStyle* aStyle,
		gfxTextPerfMetrics* aTextPerf,
		gfxUserFontSet* aUserFontSet,
		float aDevToCssSize);

LIBRARY_API void moz2d_font_group_metrics (gfxFontGroup* aFontGroup, gfxFont::Metrics* aMetrics, bool isVertical);

/**
 * Text run creation helper for clients that don't want to pass
 * a full Parameters record.
 * aText must be represented in ASCII format
 */
LIBRARY_API gfxTextRun* moz2d_font_group_make_text_run_ascii (
		DrawTarget* drawTarget,
		gfxFontGroup* aFontGroup,
		const uint8_t* aText,
		uint32_t aLength,
		uint32_t* initialBreaks,
		uint32_t initialBreakCount,
		int32_t aAppUnitsPerDevUnit,
		uint32_t aTextRunFactoryFlags);

/**
 * Text run creation helper for clients that don't want to pass
 * a full Parameters record.
 * aText must be represented in UTF16 format
 */
LIBRARY_API gfxTextRun* moz2d_font_group_make_text_run_utf16 (
		DrawTarget* drawTarget,
		gfxFontGroup* aFontGroup,
		const char16_t* aText,
		uint32_t aLength,
		uint32_t* initialBreaks,
		uint32_t initialBreakCount,
		int32_t aAppUnitsPerDevUnit,
		uint32_t aTextRunFactoryFlags);

#ifdef __cplusplus
}
#endif

#endif /* FONT_GROUP_H_ */
