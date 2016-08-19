/*
 * text_run.cpp
 *
 *  Created on: Aug 2, 2016
 *      Author: Aliaksei Syrel
 */


#include "text_run.h"
#include "gfxContext.h"


void moz2d_text_run_draw(DrawTarget* drawTarget, gfxTextRun* aTextRun, float x, float y, DrawMode drawMode, DrawOptions* drawOptions, StrokeOptions* strokeOptions, float r, float g, float b, float a) {
	Matrix transform = drawTarget->GetTransform();

	RefPtr<gfxContext> aContext = gfxContext::CreatePreservingTransformOrNull(drawTarget);

	gfxTextRun::DrawParams aParams = gfxTextRun::DrawParams(aContext);
	aParams.drawOpts = drawOptions;
	aParams.strokeOpts = strokeOptions;
	aParams.textStrokeColor = NS_RGBA((unsigned char)(r*255), (unsigned char)(g*255), (unsigned char)(b*255),(unsigned char)(a*255));
	aParams.drawMode = drawMode;

	aTextRun->Draw(gfxTextRun::Range(aTextRun), gfxPoint(x,y),aParams);

	drawTarget->SetTransform(transform);
}

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
		gfxBreakPriority *aBreakPriority) {
	return aTextRun->BreakAndMeasureText(
			aStart,
			aMaxLength,
			aLineBreakBefore,
			aWidth,
			aProvider,
			aSuppressBreak,
			aTrimWhitespace,
			aMetrics,
			aBoundingBoxType,
			aDrawTargetForTightBoundingBox,
			aUsedHyphenation,
			aLastBreak,
			aCanWordWrap,
			aBreakPriority);
}
