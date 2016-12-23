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
        : hyphensOption(0),
          appUnits(0),
          hyphenWidth(0),
          drawTarget(0)
    {}
	int8_t hyphensOption;
    uint32_t appUnits;
    gfxFloat hyphenWidth;
    DrawTarget* drawTarget;
};

struct SpacingCollector final {
    SpacingCollector()
            : start(0),
              end(0),
              spacing(nullptr)
    {}
    uint32_t start;
    uint32_t end;
    double* spacing;
};

struct BreaksCollector final {
    BreaksCollector()
            : start(0),
              end(0),
              breaks(nullptr)
    {}
    uint32_t start;
    uint32_t end;
    bool* breaks;
};

class PluggablePropertyProvider : public gfxTextRun::PropertyProvider {

public:
	void SetGetHyphenationBreaks(void (*getHyphenationBreaks)(std::uintptr_t, BreaksCollector*)) {
		mGetHyphenationBreaks = getHyphenationBreaks;
	}

	void SetGetHyphensOption(void (*getHyphensOption)(std::uintptr_t, PropertyCollector*)) {
		mGetHyphensOption = getHyphensOption;
	}

	void SetGetHyphenWidth(void (*getHyphenWidth)(std::uintptr_t, PropertyCollector*)) {
		mGetHyphenWidth = getHyphenWidth;
	}

	void SetGetSpacing(void (*getSpacing)(std::uintptr_t, SpacingCollector*)) {
		mGetSpacing = getSpacing;
	}

	void SetGetDrawTarget(void (*getDrawTarget)(std::uintptr_t, PropertyCollector*)) {
		mGetDrawTarget = getDrawTarget;
	}

	void SetGetAppUnitsPerDevUnit(void (*getAppUnitsPerDevUnit)(std::uintptr_t, PropertyCollector*)) {
		mGetAppUnitsPerDevUnit = getAppUnitsPerDevUnit;
	}

	PluggablePropertyProvider(std::uintptr_t smalltalkPtr) {
		mSmalltalkPtr = smalltalkPtr;
	}

	virtual ~PluggablePropertyProvider() {}

	virtual void GetHyphenationBreaks(gfxTextRun::Range aRange, bool *aBreakBefore) {
        BreaksCollector collector;
        collector.start = aRange.start;
        collector.end = aRange.end;

        // we use already allocated array of breaks
        collector.breaks = aBreakBefore;
        uint32_t index;
        for (index = 0; index < aRange.Length(); ++index) {
            collector.breaks[index] = false;
        }

        mGetHyphenationBreaks(mSmalltalkPtr, &collector);
        collector.breaks = nullptr;
	}

	virtual int8_t GetHyphensOption() {
		PropertyCollector collector;
		mGetHyphensOption(mSmalltalkPtr, &collector);
		return collector.hyphensOption;
	}

	virtual gfxFloat GetHyphenWidth() {
        PropertyCollector collector;
        mGetHyphenWidth(mSmalltalkPtr, &collector);
        return collector.hyphenWidth;
	}

	virtual void GetSpacing(gfxTextRun::Range aRange, Spacing *aSpacing) {

        SpacingCollector collector;
        collector.start = aRange.start;
        collector.end = aRange.end;
        collector.spacing = new double[aRange.Length() * 2];
        uint32_t index;
        for (index = 0; index < aRange.Length() * 2; ++index) {
            collector.spacing[index] = 0;
        }

		mGetSpacing(mSmalltalkPtr, &collector);

		for (index = 0; index < aRange.Length(); ++index) {
            aSpacing[index].mBefore = collector.spacing[index * 2];
            aSpacing[index].mAfter = collector.spacing[index * 2 + 1];
        }

        delete[] collector.spacing;
        collector.spacing = nullptr;
	}

	virtual already_AddRefed<DrawTarget> GetDrawTarget() {
        PropertyCollector collector;
        mGetDrawTarget(mSmalltalkPtr, &collector);
		RefPtr<DrawTarget> drawTarget = collector.drawTarget;
		return drawTarget.forget();
	}

	virtual uint32_t GetAppUnitsPerDevUnit() {
        PropertyCollector collector;
        mGetAppUnitsPerDevUnit(mSmalltalkPtr, &collector);
        return collector.appUnits;
	}
	
	bool isValid() {
		return mGetHyphenationBreaks
			&& mGetHyphensOption
			&& mGetHyphenWidth
			&& mGetSpacing
			&& mGetDrawTarget
			&& mGetAppUnitsPerDevUnit;
	}

    std::uintptr_t getSmalltalkPtr() {
        return mSmalltalkPtr;
    }

private:
	void (*mGetHyphenationBreaks)(std::uintptr_t, BreaksCollector*);
	void (*mGetHyphensOption)(std::uintptr_t, PropertyCollector*);
	void (*mGetHyphenWidth)(std::uintptr_t, PropertyCollector*);
	void (*mGetSpacing)(std::uintptr_t, SpacingCollector*);
	void (*mGetDrawTarget)(std::uintptr_t, PropertyCollector*);
	void (*mGetAppUnitsPerDevUnit)(std::uintptr_t, PropertyCollector*);
	std::uintptr_t mSmalltalkPtr = 0;	// identity hash of smalltalk property provider
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
LIBRARY_API void moz2d_text_run_property_collector_set_app_units (PropertyCollector* propertyCollector, uint32_t appUnits);
LIBRARY_API void moz2d_text_run_property_collector_set_hyphen_width (PropertyCollector* propertyCollector, gfxFloat hyphenWidth);
LIBRARY_API void moz2d_text_run_property_collector_set_draw_target (PropertyCollector* propertyCollector, DrawTarget* drawTarget);
LIBRARY_API double* moz2d_text_run_spacing_collector_get_spacing (SpacingCollector* propertyCollector);
LIBRARY_API uint32_t moz2d_text_run_spacing_collector_get_start (SpacingCollector* propertyCollector);
LIBRARY_API uint32_t moz2d_text_run_spacing_collector_get_end (SpacingCollector* propertyCollector);
LIBRARY_API bool* moz2d_text_run_breaks_collector_get_breaks (BreaksCollector* propertyCollector);
LIBRARY_API uint32_t moz2d_text_run_breaks_collector_get_start (BreaksCollector* propertyCollector);
LIBRARY_API uint32_t moz2d_text_run_breaks_collector_get_end (BreaksCollector* propertyCollector);

LIBRARY_API PluggablePropertyProvider* moz2d_text_run_property_provider_create(std::uintptr_t smalltalkPtr);

LIBRARY_API void moz2d_text_run_property_provider_init (
		PluggablePropertyProvider* propertyProvider,
		void (*getHyphenationBreaks)(std::uintptr_t, BreaksCollector*),
		void (*getHyphensOption)(std::uintptr_t, PropertyCollector*),
		void (*getHyphenWidth)(std::uintptr_t, PropertyCollector*),
		void (*getSpacing)(std::uintptr_t, SpacingCollector*),
		void (*getDrawTarget)(std::uintptr_t, PropertyCollector*),
		void (*getAppUnitsPerDevUnit)(std::uintptr_t, PropertyCollector*));

LIBRARY_API std::uintptr_t moz2d_text_run_property_provider_get_smalltalk_ptr (PluggablePropertyProvider* propertyProvider);
LIBRARY_API gfxFloat moz2d_text_run_property_provider_get_hyphen_width (PluggablePropertyProvider* propertyProvider);
LIBRARY_API uint32_t moz2d_text_run_property_provider_get_app_units (PluggablePropertyProvider* propertyProvider);
LIBRARY_API void moz2d_text_run_property_provider_get_hyphenation_breaks (PluggablePropertyProvider* propertyProvider, uint32_t start, uint32_t end, bool * aBreakBefore);
LIBRARY_API void moz2d_text_run_property_provider_get_spacing (PluggablePropertyProvider* propertyProvider, uint32_t start, uint32_t end, double * spacing);
LIBRARY_API void moz2d_text_run_property_provider_get_breaks (PluggablePropertyProvider* propertyProvider, uint32_t start, uint32_t end, bool * breaks);
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
