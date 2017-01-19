/*
 * text_run.cpp
 *
 *  Created on: Aug 2, 2016
 *      Author: Aliaksei Syrel
 */


#include "text_run.h"
#include "gfxContext.h"
#include "gfx2DGlue.h"

already_AddRefed<gfxPattern> ThebesPattern (Pattern* aPattern) {
	RefPtr<gfxPattern> pattern;
	switch(aPattern->GetType()) {
		case PatternType::COLOR: {
			ColorPattern* colorPattern = reinterpret_cast<ColorPattern*>(aPattern);
			pattern = new gfxPattern(colorPattern->mColor);
			break;
		}
		case PatternType::SURFACE: {
			SurfacePattern* surfacePattern = reinterpret_cast<SurfacePattern*>(aPattern);
			pattern = new gfxPattern(surfacePattern->mSurface, surfacePattern->mMatrix);
			pattern->SetSamplingFilter(surfacePattern->mSamplingFilter);
			pattern->SetExtend(surfacePattern->mExtendMode);
			break;
		}
		case PatternType::LINEAR_GRADIENT: {
			LinearGradientPattern* linearGradient = reinterpret_cast<LinearGradientPattern*>(aPattern);
			pattern = new gfxPattern(linearGradient->mBegin.x, linearGradient->mBegin.y, linearGradient->mEnd.x, linearGradient->mEnd.y);
			pattern->SetMatrix(ThebesMatrix(linearGradient->mMatrix));
			pattern->SetColorStops(linearGradient->mStops);
			break;
		}
		case PatternType::RADIAL_GRADIENT: {
			RadialGradientPattern* radialGradient = reinterpret_cast<RadialGradientPattern*>(aPattern);
			pattern = new gfxPattern(radialGradient->mCenter1.x, radialGradient->mCenter1.y, radialGradient->mRadius1, radialGradient->mCenter2.x, radialGradient->mCenter2.y, radialGradient->mRadius2);
			pattern->SetMatrix(ThebesMatrix(radialGradient->mMatrix));
			pattern->SetColorStops(radialGradient->mStops);
			break;
		}
	}
	return pattern.forget();
}

void moz2d_text_run_draw_pattern (
		DrawTarget* drawTarget,
		gfxTextRun* aTextRun,
		uint32_t start,
		uint32_t end,
		gfxTextRun::PropertyProvider *aProvider,
		gfxFloat x,
		gfxFloat y,
		DrawMode aDrawMode,
		DrawOptions* drawOptions,
		StrokeOptions* strokeOptions,
		Pattern* aFillPattern,
		Pattern* aStrokePattern) {
	Matrix transform = drawTarget->GetTransform();

	RefPtr<gfxContext> aContext = gfxContext::CreatePreservingTransformOrNull(drawTarget);
	RefPtr<gfxPattern> fillPattern = ThebesPattern(aFillPattern);
	RefPtr<gfxPattern> strokePattern = ThebesPattern(aStrokePattern);
	aContext->SetPattern(fillPattern);

	gfxTextRun::DrawParams aParams = gfxTextRun::DrawParams(aContext);
	aParams.drawOpts = drawOptions;
	aParams.strokeOpts = strokeOptions;
	aParams.textStrokePattern = strokePattern;
	aParams.drawMode = aDrawMode;
	aParams.provider = aProvider;

	aTextRun->Draw(gfxTextRun::Range(start - 1, end), gfxPoint(x,y),aParams);
	drawTarget->SetTransform(transform);
}

void moz2d_text_run_draw_color (
		DrawTarget* drawTarget,
		gfxTextRun* aTextRun,
		uint32_t start,
		uint32_t end,
        gfxTextRun::PropertyProvider *aProvider,
		gfxFloat x,
		gfxFloat y,
		DrawMode drawMode,
		DrawOptions* drawOptions,
		StrokeOptions* strokeOptions,
		float fillR,
		float fillG,
		float fillB,
		float fillA,
		float strokeR,
		float strokeG,
		float strokeB,
		float strokeA) {
	Matrix transform = drawTarget->GetTransform();

	RefPtr<gfxContext> aContext = gfxContext::CreatePreservingTransformOrNull(drawTarget);
	aContext->SetColor(Color(fillR, fillG, fillB, fillA));

	gfxTextRun::DrawParams aParams = gfxTextRun::DrawParams(aContext);
	aParams.drawOpts = drawOptions;
	aParams.strokeOpts = strokeOptions;
	aParams.textStrokeColor = NS_RGBA((unsigned char)(strokeR*255), (unsigned char)(strokeG*255), (unsigned char)(strokeB*255),(unsigned char)(strokeA*255));
	aParams.drawMode = drawMode;
	aParams.provider = aProvider;

	aTextRun->Draw(gfxTextRun::Range(start - 1, end), gfxPoint(x,y),aParams);
	drawTarget->SetTransform(transform);
}

PluggablePropertyProvider* moz2d_text_run_property_provider_create(std::uintptr_t smalltalkPtr) {
	return new PluggablePropertyProvider(smalltalkPtr);
}

void moz2d_text_run_property_provider_init (
		PluggablePropertyProvider* propertyProvider,
		void (*getHyphenationBreaks)(std::uintptr_t, BreaksCollector*),
		void (*getHyphenWidth)(std::uintptr_t, PropertyCollector*),
		void (*getSpacing)(std::uintptr_t, SpacingCollector*),
		void (*getDrawTarget)(std::uintptr_t, PropertyCollector*),
		void (*getAppUnitsPerDevUnit)(std::uintptr_t, PropertyCollector*)) {
	propertyProvider->SetGetHyphenationBreaks(getHyphenationBreaks);
	propertyProvider->SetGetHyphenWidth(getHyphenWidth);
	propertyProvider->SetGetSpacing(getSpacing);
	propertyProvider->SetGetDrawTarget(getDrawTarget);
	propertyProvider->SetGetAppUnitsPerDevUnit(getAppUnitsPerDevUnit);
}

void moz2d_text_run_property_collector_set_app_units (PropertyCollector* propertyCollector, uint32_t appUnits) {
    propertyCollector->appUnits = appUnits;
}

void moz2d_text_run_property_collector_set_hyphen_width (PropertyCollector* propertyCollector, gfxFloat hyphenWidth) {
    propertyCollector->hyphenWidth = hyphenWidth;
}

void moz2d_text_run_property_collector_set_draw_target (PropertyCollector* propertyCollector, DrawTarget* drawTarget) {
    propertyCollector->drawTarget = drawTarget;
}

gfxFloat* moz2d_text_run_spacing_collector_get_spacing (SpacingCollector* propertyCollector) {
    return propertyCollector->spacing;
}

uint32_t moz2d_text_run_spacing_collector_get_start (SpacingCollector* propertyCollector) {
    return propertyCollector->start;
}

uint32_t moz2d_text_run_spacing_collector_get_end (SpacingCollector* propertyCollector) {
    return propertyCollector->end;
}

bool* moz2d_text_run_breaks_collector_get_breaks (BreaksCollector* propertyCollector) {
    return propertyCollector->breaks;
}

uint32_t moz2d_text_run_breaks_collector_get_start (BreaksCollector* propertyCollector) {
    return propertyCollector->start;
}

uint32_t moz2d_text_run_breaks_collector_get_end (BreaksCollector* propertyCollector){
    return propertyCollector->end;
}

gfxFloat moz2d_text_run_property_provider_get_hyphen_width (PluggablePropertyProvider* propertyProvider) {
	return propertyProvider->GetHyphenWidth();
}

uint32_t moz2d_text_run_property_provider_get_app_units (PluggablePropertyProvider* propertyProvider) {
    return propertyProvider->GetAppUnitsPerDevUnit();
}

std::uintptr_t moz2d_text_run_property_provider_get_smalltalk_ptr (PluggablePropertyProvider* propertyProvider) {
    return propertyProvider->getSmalltalkPtr();
}

void moz2d_text_run_property_provider_get_hyphenation_breaks (PluggablePropertyProvider* propertyProvider, uint32_t start, uint32_t end, bool * aBreakBefore) {
	gfxTextRun::Range range(start, end);
	propertyProvider->GetHyphenationBreaks(range, aBreakBefore);
}

void moz2d_text_run_property_provider_get_spacing (PluggablePropertyProvider* propertyProvider, uint32_t start, uint32_t end, gfxFloat * spacing) {
    gfxTextRun::Range range(start, end);
    PluggablePropertyProvider::Spacing* aSpacing = new PluggablePropertyProvider::Spacing[range.Length()];
    propertyProvider->GetSpacing(range, aSpacing);

    uint32_t index;
    for (index = 0; index < range.Length(); ++index) {
        spacing[index * 2] = aSpacing[index].mBefore;
        spacing[index * 2 + 1] = aSpacing[index].mAfter;
    }

    delete[] aSpacing;
}

void moz2d_text_run_property_provider_get_breaks (PluggablePropertyProvider* propertyProvider, uint32_t start, uint32_t end, bool * breaks) {
    gfxTextRun::Range range(start, end);
    propertyProvider->GetHyphenationBreaks(range, breaks);
}

DrawTarget* moz2d_text_run_property_provider_get_draw_target (PluggablePropertyProvider* propertyProvider) {
	return (propertyProvider->GetDrawTarget()).take();
}

bool moz2d_text_run_property_provider_is_valid (PluggablePropertyProvider* propertyProvider) {
	return propertyProvider->isValid();
}

int8_t moz2d_text_run_property_provider_get_hyphens_option (PluggablePropertyProvider* propertyProvider) {
	return propertyProvider->GetHyphensOption();
}

void moz2d_text_run_measure_text (
		DrawTarget *aDrawTarget,
		gfxTextRun* aTextRun,
		gfxTextRun::PropertyProvider* aProvider,
		TextRunMetrics* aMetrics,
		gfxFont::BoundingBoxType aBoundingBoxType) {
	gfxTextRun::Metrics metrics = aTextRun->MeasureText(aBoundingBoxType, aDrawTarget, aProvider);
	aMetrics->mAdvanceWidth = metrics.mAdvanceWidth;
	aMetrics->mAscent = metrics.mAscent;
	aMetrics->mDescent = metrics.mDescent;
	aMetrics->mBoundingBoxX = metrics.mBoundingBox.x;
	aMetrics->mBoundingBoxY = metrics.mBoundingBox.y;
	aMetrics->mBoundingBoxWidth = metrics.mBoundingBox.width;
	aMetrics->mBoundingBoxHeight = metrics.mBoundingBox.height;
}

void moz2d_text_run_font_metrics (gfxTextRun* aTextRun, gfxFont::Metrics* aMetrics) {
	*aMetrics = (aTextRun->GetFontGroup()->GetFirstValidFont())->GetMetrics(aTextRun->IsVertical() ? gfxFont::Orientation::eVertical : gfxFont::Orientation::eHorizontal);
	aMetrics->underlineOffset = (aTextRun->GetFontGroup())->GetUnderlineOffset();
}

void moz2d_text_run_property_provider_set_hyphens_option (PluggablePropertyProvider* propertyProvider, int8_t aHyphensOption) {
    propertyProvider->SetHyphensOption(aHyphensOption);
}

void moz2d_text_run_property_provider_delete (PluggablePropertyProvider* propertyProvider) {
	delete propertyProvider;
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
		TextRunMetrics *aMetrics,
		gfxFont::BoundingBoxType aBoundingBoxType,
		DrawTarget* aDrawTargetForTightBoundingBox,
		bool *aUsedHyphenation,
		uint32_t *aLastBreak,
		bool aCanWordWrap,
		gfxBreakPriority *aBreakPriority) {

	gfxTextRun::Metrics* metrics = new gfxTextRun::Metrics();

	uint32_t result = aTextRun->BreakAndMeasureText(
			aStart,
			aMaxLength,
			aLineBreakBefore,
			aWidth,
			aProvider,
			aSuppressBreak,
			aTrimWhitespace,
            true, //TODO what is this?
			metrics,
			aBoundingBoxType,
			aDrawTargetForTightBoundingBox,
			aUsedHyphenation,
			aLastBreak,
			aCanWordWrap,
			aBreakPriority);

	aMetrics->mAdvanceWidth = metrics->mAdvanceWidth;
	aMetrics->mAscent = metrics->mAscent;
	aMetrics->mDescent = metrics->mDescent;
	aMetrics->mBoundingBoxX = metrics->mBoundingBox.x;
	aMetrics->mBoundingBoxY = metrics->mBoundingBox.y;
	aMetrics->mBoundingBoxWidth = metrics->mBoundingBox.width;
	aMetrics->mBoundingBoxHeight = metrics->mBoundingBox.height;
	return result;
}

uint32_t moz2d_text_run_get_length(gfxTextRun* aTextRun) {
	return aTextRun->GetLength();
}

void moz2d_text_run_release(gfxTextRun* aTextRun) {
    aTextRun->Release();
}

MozRefCountType moz2d_text_run_ref_count(gfxTextRun* aTextRun) {
    return HackTextRun::refCount(aTextRun);
}