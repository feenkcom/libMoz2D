/*
 * text_run.h
 *
 *  Created on: Aug 2, 2016
 *      Author: Aliaksei Syrel
 */

#ifndef TEXT_RUN_H_
#define TEXT_RUN_H_

#include "2d/2D.h"
#include "gfxTextRun.h"

using namespace mozilla::gfx;

#ifdef __cplusplus
extern "C" {
#endif

void moz2d_text_run_draw(
		DrawTarget* drawTarget,
		gfxTextRun* aTextRun,
		float x,
		float y,
		DrawMode drawMode,
		DrawOptions* drawOptions,
		StrokeOptions* strokeOptions,
		float r,
		float g,
		float b,
		float a);

uint32_t moz2d_text_run_break_and_measure (
		gfxTextRun* aTextRun,
		uint32_t aStart,
		uint32_t aMaxLength,
		bool aLineBreakBefore,
		gfxFloat aWidth,
		gfxTextRun::PropertyProvider *aProvider,
		gfxTextRun::SuppressBreak aSuppressBreak,
		gfxFloat *aTrimWhitespace,
		gfxTextRun::Metrics *aMetrics,
		gfxFont::BoundingBoxType aBoundingBoxType,
		DrawTarget* aDrawTargetForTightBoundingBox,
		bool *aUsedHyphenation,
		uint32_t *aLastBreak,
		bool aCanWordWrap,
		gfxBreakPriority *aBreakPriority);


#ifdef __cplusplus
}
#endif

#endif /* TEXT_RUN_H_ */
