/*
 * text_run.h
 *
 *  Created on: Aug 2, 2016
 *      Author: Aliaksei Syrel
 */

#ifndef TEXT_RUN_H_
#define TEXT_RUN_H_

#include "exports.h"
#include "2d/2D.h"
#include "gfxTextRun.h"
#include <stdint.h>

using namespace mozilla;
#include "layout/generic/nsTextRunTransformations.h"

using namespace mozilla::gfx;


struct PropertyCollector final {
	PropertyCollector()
        : hyphensOption(0)
    {}
	void setHyphensOption (int8_t option) {
		hyphensOption = option;
	}
	int8_t	hyphensOption;
};

class PluggablePropertyProvider : public gfxTextRun::PropertyProvider {

public:
	void SetGetHyphenationBreaks(void (*getHyphenationBreaks)(uint32_t, uint32_t, bool *)) {
		mGetHyphenationBreaks = getHyphenationBreaks;
	}

	void SetGetHyphensOption(void (*getHyphensOption)(PropertyCollector*)) {
		mGetHyphensOption = getHyphensOption;
	}

	void SetGetHyphenWidth(gfxFloat (*getHyphenWidth)(void)) {
		mGetHyphenWidth = getHyphenWidth;
	}

	void SetGetSpacing(void (*getSpacing)(uint32_t, uint32_t, double*)) {
		mGetSpacing = getSpacing;
	}

	void SetGetDrawTarget(void (*getDrawTarget)(std::uintptr_t*)) {
		mGetDrawTarget = getDrawTarget;
	}

	void SetGetAppUnitsPerDevUnit(uint32_t (*getAppUnitsPerDevUnit)(void)) {
		mGetAppUnitsPerDevUnit = getAppUnitsPerDevUnit;
	}

	virtual ~PluggablePropertyProvider() {}

	virtual void GetHyphenationBreaks(gfxTextRun::Range aRange, bool *aBreakBefore) {
		mGetHyphenationBreaks(aRange.start, aRange.end, aBreakBefore);
	}

	virtual int8_t GetHyphensOption() {
		PropertyCollector collector;
		collector.hyphensOption = 0;
		mGetHyphensOption(&collector);
		return collector.hyphensOption;
	}

	virtual gfxFloat GetHyphenWidth() {
		return mGetHyphenWidth();
	}

	virtual void GetSpacing(gfxTextRun::Range aRange, Spacing *aSpacing) {

		double* spacing = new double[aRange.Length() * 2];
		uint32_t index;
		for (index = 0; index < aRange.Length() * 2; ++index) {
			spacing[index] = 0;
		}

		mGetSpacing(aRange.start, aRange.end, spacing);

		for (index = 0; index < aRange.Length(); ++index) {
			aSpacing[index].mBefore = spacing[index * 2];
			aSpacing[index].mAfter = spacing[index * 2 + 1];
		}
	}

	virtual already_AddRefed<DrawTarget> GetDrawTarget() {
		std::uintptr_t drawTargetPtr = 0;
		mGetDrawTarget(&drawTargetPtr);
		DrawTarget* rawDrawTarget = reinterpret_cast<DrawTarget*>(drawTargetPtr);
		RefPtr<DrawTarget> drawTarget = rawDrawTarget;
		return drawTarget.forget();
	}

	virtual uint32_t GetAppUnitsPerDevUnit() {
		return mGetAppUnitsPerDevUnit();
	}
	
	bool isValid() {
		return mGetHyphenationBreaks
			&& mGetHyphensOption
			&& mGetHyphenWidth
			&& mGetSpacing
			&& mGetDrawTarget
			&& mGetAppUnitsPerDevUnit;
	}

private:
	void (*mGetHyphenationBreaks)(uint32_t, uint32_t, bool *);
	void (*mGetHyphensOption)(PropertyCollector*);
	gfxFloat (*mGetHyphenWidth)(void);
	void (*mGetSpacing)(uint32_t, uint32_t, double*);
	void (*mGetDrawTarget)(std::uintptr_t*);
	uint32_t (*mGetAppUnitsPerDevUnit)(void);
};

struct TextRunMetrics {
	gfxFloat mAdvanceWidth;
	gfxFloat mAscent;  // always non-negative
	gfxFloat mDescent; // always non-negative
	gfxFloat mBoundingBoxX;
	gfxFloat mBoundingBoxY;
	gfxFloat mBoundingBoxWidth;
	gfxFloat mBoundingBoxHeight;
};

#ifdef __cplusplus
extern "C" {
#endif

LIBRARY_API void moz2d_text_run_property_collector_set_hyphens_option (PropertyCollector* propertyCollector, int8_t hyphensOption);

LIBRARY_API PluggablePropertyProvider* moz2d_text_run_property_provider_create();

LIBRARY_API void moz2d_text_run_property_provider_init (
		PluggablePropertyProvider* propertyProvider,
		void (*getHyphenationBreaks)(uint32_t, uint32_t, bool *),
		void (*getHyphensOption)(PropertyCollector*),
		gfxFloat (*getHyphenWidth)(void),
		void (*getSpacing)(uint32_t, uint32_t, double*),
		void (*getDrawTarget)(std::uintptr_t*),
		uint32_t (*getAppUnitsPerDevUnit)(void));

LIBRARY_API gfxFloat moz2d_text_run_property_provider_get_hyphen_width (PluggablePropertyProvider* propertyProvider);
LIBRARY_API void moz2d_text_run_property_provider_get_hyphenation_breaks (PluggablePropertyProvider* propertyProvider, uint32_t start, uint32_t end, bool * aBreakBefore);
LIBRARY_API DrawTarget* moz2d_text_run_property_provider_get_draw_target (PluggablePropertyProvider* propertyProvider);
LIBRARY_API bool moz2d_text_run_property_provider_is_valid (PluggablePropertyProvider* propertyProvider);
LIBRARY_API int8_t moz2d_text_run_property_provider_get_hyphens_option (PluggablePropertyProvider* propertyProvider);

LIBRARY_API void moz2d_text_run_property_provider_delete (PluggablePropertyProvider* propertyProvider);

LIBRARY_API void moz2d_text_run_draw_pattern (
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
		Pattern* fillPattern,
		Pattern* aStrokePattern);

LIBRARY_API void moz2d_text_run_draw_color (
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
		float fillR,
		float fillG,
		float fillB,
		float fillA,
		float strokeR,
		float strokeG,
		float strokeB,
		float strokeA);

LIBRARY_API void moz2d_text_run_measure_text (
        DrawTarget *aDrawTarget,
		gfxTextRun *aTextRun,
        gfxTextRun::PropertyProvider *aProvider,
        TextRunMetrics *aMetrics,
		gfxFont::BoundingBoxType aBoundingBoxType);

LIBRARY_API void moz2d_text_run_font_metrics (gfxTextRun* aTextRun, gfxFont::Metrics* aMetrics);

LIBRARY_API uint32_t moz2d_text_run_break_and_measure (
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
		gfxBreakPriority *aBreakPriority);

LIBRARY_API uint32_t moz2d_text_run_get_length(gfxTextRun* aTextRun);

LIBRARY_API nsTransformingTextRunFactory* moz2d_text_run_create_math_ml_factory();

LIBRARY_API gfxTextRun* moz2d_text_run_factory_make_text_run_utf16 (
		nsTransformingTextRunFactory* aTransformingFactory,
		FontFamilyList* aFontFamilyList,
		DrawTarget* drawTarget,
		gfxFontGroup* aFontGroup,
		const char16_t* aText,
		int32_t aLength,
		uint32_t* initialBreaks,
		uint32_t initialBreakCount,
		int32_t aAppUnitsPerDevUnit,
		uint32_t aTextRunFactoryFlags);

#ifdef __cplusplus
}
#endif

#endif /* TEXT_RUN_H_ */
