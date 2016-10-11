/*
 * text_run.cpp
 *
 *  Created on: Aug 2, 2016
 *      Author: Aliaksei Syrel
 */


#include "text_run.h"
#include "gfxContext.h"
#include "nsTArray.h"
#include "mozilla/UniquePtr.h"
#include "layout/generic/MathMLTextRunFactory.h"
#include "layout/base/nsPresContext.h"
#include "undefined/nsFakePresContext.h"
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
		default: {
			pattern = nullptr;
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

PluggablePropertyProvider* moz2d_text_run_property_provider_create() {
	return new PluggablePropertyProvider();
}

void moz2d_text_run_property_provider_init (
		PluggablePropertyProvider* propertyProvider,
		void (*getHyphenationBreaks)(uint32_t, uint32_t, bool *),
		void (*getHyphensOption)(PropertyCollector*),
		gfxFloat (*getHyphenWidth)(void),
		void (*getSpacing)(uint32_t, uint32_t, double*),
		void (*getDrawTarget)(std::uintptr_t*),
		uint32_t (*getAppUnitsPerDevUnit)(void)) {
	propertyProvider->SetGetHyphenationBreaks(getHyphenationBreaks);
	propertyProvider->SetGetHyphensOption(getHyphensOption);
	propertyProvider->SetGetHyphenWidth(getHyphenWidth);
	propertyProvider->SetGetSpacing(getSpacing);
	propertyProvider->SetGetDrawTarget(getDrawTarget);
	propertyProvider->SetGetAppUnitsPerDevUnit(getAppUnitsPerDevUnit);
}

void moz2d_text_run_property_collector_set_hyphens_option (PropertyCollector* propertyCollector, int8_t hyphensOption) {
	propertyCollector->hyphensOption = hyphensOption;
}

gfxFloat moz2d_text_run_property_provider_get_hyphen_width (PluggablePropertyProvider* propertyProvider) {
	return propertyProvider->GetHyphenWidth();
}

void moz2d_text_run_property_provider_get_hyphenation_breaks (PluggablePropertyProvider* propertyProvider, uint32_t start, uint32_t end, bool * aBreakBefore) {
	gfxTextRun::Range range(start, end);
	propertyProvider->GetHyphenationBreaks(range, aBreakBefore);
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

/* --------------------------------------------------- */
/* ---------------- E X P E R I M E N T -------------- */
/* --------------------------------------------------- */
nsTransformingTextRunFactory* moz2d_text_run_create_math_ml_factory() {
	mozilla::UniquePtr<nsTransformingTextRunFactory> transformingFactory;
	transformingFactory = MakeUnique<MathMLTextRunFactory>(Move(transformingFactory), MathMLTextRunFactory::MATH_FONT_FEATURE_DTLS | MathMLTextRunFactory::MATH_FONT_STYLING_NORMAL, 1, 1.0);
	return transformingFactory.release();
}

class nsTransformedCharStyleMy {

	NS_INLINE_DECL_REFCOUNTING(nsTransformedCharStyle)

	nsFont                  mFont;
	nsCOMPtr<nsIAtom>       mLanguage;
	RefPtr<nsPresContext>   mPresContext;
	float                   mScriptSizeMultiplier;
	uint8_t                 mTextTransform;
	uint8_t                 mMathVariant;
	bool                    mExplicitLanguage;
	bool                    mForceNonFullWidth = false;

private:
	~nsTransformedCharStyleMy() {}
	nsTransformedCharStyleMy& operator=(const nsTransformedCharStyle& aOther) = delete;
};


gfxTextRun* moz2d_text_run_factory_make_text_run_utf16 (
		nsTransformingTextRunFactory* aTransformingFactory,
		FontFamilyList* aFontFamilyList,
		DrawTarget* drawTarget,
		gfxFontGroup* aFontGroup,
		const char16_t* aText,
		int32_t aLength,
		uint32_t* initialBreaks,
		uint32_t initialBreakCount,
		int32_t aAppUnitsPerDevUnit,
		uint32_t aTextRunFactoryFlags) {

	gfxTextRunFactory::Parameters params = {
			drawTarget, nullptr, nullptr, initialBreaks, initialBreakCount, aAppUnitsPerDevUnit
	};
	aFontGroup->UpdateUserFonts();
	nsTArray<RefPtr<nsTransformedCharStyle>> styles;
	nsTransformedCharStyleMy* myCharStyle = new nsTransformedCharStyleMy();
	nsFont font = nsFont(*aFontFamilyList, aFontGroup->GetStyle()->size);
	myCharStyle->mFont = font;
	RefPtr<nsFakePresContext> fakePresContext = RefPtr<nsFakePresContext>();
	nsFakePresContext* fakePresContextPtr = fakePresContext;
	RefPtr<nsPresContext> presContext = reinterpret_cast<nsPresContext*>(fakePresContextPtr);
	myCharStyle->mPresContext = presContext;
	myCharStyle->mScriptSizeMultiplier = 0.71f;
	myCharStyle->mMathVariant = NS_MATHML_MATHVARIANT_NORMAL;
	RefPtr<nsTransformedCharStyle> charStyle = reinterpret_cast<nsTransformedCharStyle*>(myCharStyle);
	printf("charStyle->mScriptSizeMultiplier = %f\n", charStyle->mScriptSizeMultiplier);

	for (int32_t i = 0; i < aLength; ++i) {
		styles.AppendElement(charStyle);
	}

	UniquePtr<nsTransformedTextRun> textRun = aTransformingFactory->MakeTextRun(
			aText, aLength,
			&params, aFontGroup, aTextRunFactoryFlags,
			Move(styles), true);
	textRun->FinishSettingProperties(drawTarget, nullptr);
	return textRun.release();
}
