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

typedef unsigned int SmalltalkPtr;

using namespace mozilla;
using namespace mozilla::gfx;


struct PropertyCollector final {
	PropertyCollector()
        : appUnits(0),
          hyphenWidth(0),
          drawTarget(0)
    {}
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
	gfxFloat* spacing;
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
	void SetGetHyphenationBreaks(void (*getHyphenationBreaks)(SmalltalkPtr, BreaksCollector*)) {
		mGetHyphenationBreaks = getHyphenationBreaks;
	}

	void SetHyphensOption(int8_t hyphensOption) {
        mHyphensOption = hyphensOption;
	}

	void SetGetHyphenWidth(void (*getHyphenWidth)(SmalltalkPtr, PropertyCollector*)) {
		mGetHyphenWidth = getHyphenWidth;
	}

	void SetGetSpacing(void (*getSpacing)(SmalltalkPtr, SpacingCollector*)) {
		mGetSpacing = getSpacing;
	}

	void SetGetDrawTarget(void (*getDrawTarget)(SmalltalkPtr, PropertyCollector*)) {
		mGetDrawTarget = getDrawTarget;
	}

	void SetGetAppUnitsPerDevUnit(void (*getAppUnitsPerDevUnit)(SmalltalkPtr, PropertyCollector*)) {
		mGetAppUnitsPerDevUnit = getAppUnitsPerDevUnit;
	}

	PluggablePropertyProvider(SmalltalkPtr smalltalkPtr) {
		mSmalltalkPtr = smalltalkPtr;
	}

	virtual ~PluggablePropertyProvider() {}

	// Callback is optional
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
		if (mGetHyphenationBreaks) {
			mGetHyphenationBreaks(mSmalltalkPtr, &collector);
		}
		// we didn't allocate anyspace and just reused passed pointer to an array
        collector.breaks = nullptr;
	}

	// Does not require a callback
	virtual int8_t GetHyphensOption() {
		return mHyphensOption;
	}

	// Callback is optional
	virtual gfxFloat GetHyphenWidth() {
        PropertyCollector collector;
		if (mGetHyphenWidth) {
			mGetHyphenWidth(mSmalltalkPtr, &collector);
		}
		return collector.hyphenWidth;
	}

	// Callback is optional
	virtual void GetSpacing(gfxTextRun::Range aRange, Spacing *aSpacing) {

        SpacingCollector collector;
        collector.start = aRange.start;
        collector.end = aRange.end;
        collector.spacing = new gfxFloat[aRange.Length() * 2];
        uint32_t index;
        for (index = 0; index < aRange.Length() * 2; ++index) {
            collector.spacing[index] = 0;
        }
		
		if (mGetSpacing) {
			mGetSpacing(mSmalltalkPtr, &collector);
		}
		
		for (index = 0; index < aRange.Length(); ++index) {
            aSpacing[index].mBefore = collector.spacing[index * 2];
            aSpacing[index].mAfter = collector.spacing[index * 2 + 1];
        }

        delete[] collector.spacing;
        collector.spacing = nullptr;
	}

	// Callback is optional
	virtual already_AddRefed<DrawTarget> GetDrawTarget() {
        PropertyCollector collector;
		if (mGetDrawTarget) {
			mGetDrawTarget(mSmalltalkPtr, &collector);
		}
		else { return nullptr; };
        
		RefPtr<DrawTarget> drawTarget = collector.drawTarget;
		return drawTarget.forget();
	}

	// Callback is optional
	virtual uint32_t GetAppUnitsPerDevUnit() {
        PropertyCollector collector;
		if (mGetAppUnitsPerDevUnit) {
			mGetAppUnitsPerDevUnit(mSmalltalkPtr, &collector);
		}
        return collector.appUnits;
	}
	
	bool isValid() {		
		return mGetHyphenationBreaks
			&& mGetHyphenWidth
			&& mGetSpacing
			&& mGetDrawTarget
			&& mGetAppUnitsPerDevUnit;
	}

    SmalltalkPtr getSmalltalkPtr() {
        return mSmalltalkPtr;
    }

private:
	void (*mGetHyphenationBreaks)(SmalltalkPtr, BreaksCollector*) = nullptr;
	void (*mGetHyphenWidth)(SmalltalkPtr, PropertyCollector*) = nullptr;
	void (*mGetSpacing)(SmalltalkPtr, SpacingCollector*) = nullptr;
	void (*mGetDrawTarget)(SmalltalkPtr, PropertyCollector*) = nullptr;
	void (*mGetAppUnitsPerDevUnit)(SmalltalkPtr, PropertyCollector*) = nullptr;
    int8_t mHyphensOption = 0;
	SmalltalkPtr mSmalltalkPtr = 0;	// identity hash of smalltalk property provider
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

LIBRARY_API void moz2d_text_run_property_collector_set_app_units (PropertyCollector* propertyCollector, uint32_t appUnits);
LIBRARY_API void moz2d_text_run_property_collector_set_hyphen_width (PropertyCollector* propertyCollector, gfxFloat hyphenWidth);
LIBRARY_API void moz2d_text_run_property_collector_set_draw_target (PropertyCollector* propertyCollector, DrawTarget* drawTarget);
LIBRARY_API double* moz2d_text_run_spacing_collector_get_spacing (SpacingCollector* propertyCollector);
LIBRARY_API uint32_t moz2d_text_run_spacing_collector_get_start (SpacingCollector* propertyCollector);
LIBRARY_API uint32_t moz2d_text_run_spacing_collector_get_end (SpacingCollector* propertyCollector);
LIBRARY_API bool* moz2d_text_run_breaks_collector_get_breaks (BreaksCollector* propertyCollector);
LIBRARY_API uint32_t moz2d_text_run_breaks_collector_get_start (BreaksCollector* propertyCollector);
LIBRARY_API uint32_t moz2d_text_run_breaks_collector_get_end (BreaksCollector* propertyCollector);

LIBRARY_API PluggablePropertyProvider* moz2d_text_run_property_provider_create(SmalltalkPtr smalltalkPtr);

LIBRARY_API void moz2d_text_run_property_provider_init (
		PluggablePropertyProvider* propertyProvider,
		void (*getHyphenationBreaks)(SmalltalkPtr, BreaksCollector*),
		void (*getHyphenWidth)(SmalltalkPtr, PropertyCollector*),
		void (*getSpacing)(SmalltalkPtr, SpacingCollector*),
		void (*getDrawTarget)(SmalltalkPtr, PropertyCollector*),
		void (*getAppUnitsPerDevUnit)(SmalltalkPtr, PropertyCollector*));
		
LIBRARY_API void moz2d_text_run_property_provider_init_draw_target_callback (
		PluggablePropertyProvider* propertyProvider,
		void (*getDrawTarget)(SmalltalkPtr, PropertyCollector*));

LIBRARY_API SmalltalkPtr moz2d_text_run_property_provider_get_smalltalk_ptr (PluggablePropertyProvider* propertyProvider);
LIBRARY_API gfxFloat moz2d_text_run_property_provider_get_hyphen_width (PluggablePropertyProvider* propertyProvider);
LIBRARY_API uint32_t moz2d_text_run_property_provider_get_app_units (PluggablePropertyProvider* propertyProvider);
LIBRARY_API void moz2d_text_run_property_provider_get_hyphenation_breaks (PluggablePropertyProvider* propertyProvider, uint32_t start, uint32_t end, bool * aBreakBefore);
LIBRARY_API void moz2d_text_run_property_provider_get_spacing (PluggablePropertyProvider* propertyProvider, uint32_t start, uint32_t end, double * spacing);
LIBRARY_API void moz2d_text_run_property_provider_get_breaks (PluggablePropertyProvider* propertyProvider, uint32_t start, uint32_t end, bool * breaks);
LIBRARY_API DrawTarget* moz2d_text_run_property_provider_get_draw_target (PluggablePropertyProvider* propertyProvider);
LIBRARY_API bool moz2d_text_run_property_provider_is_valid (PluggablePropertyProvider* propertyProvider);
LIBRARY_API int8_t moz2d_text_run_property_provider_get_hyphens_option (PluggablePropertyProvider* propertyProvider);

LIBRARY_API void moz2d_text_run_property_provider_set_hyphens_option (PluggablePropertyProvider* propertyProvider, int8_t aHyphensOption);
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

LIBRARY_API void moz2d_text_run_release(gfxTextRun* aTextRun);


/**
 * I am a helper class to access protected mRefCnt member for debug purposes
 */
class HackTextRun : public gfxTextRun {

public:
    HackTextRun(const gfxTextRunFactory::Parameters *aParams, uint32_t aLength, gfxFontGroup *aFontGroup,
                uint32_t aFlags) : gfxTextRun(aParams, aLength, aFontGroup, aFlags) {}

public:
    static MozRefCountType refCount(gfxTextRun *aTextRun) {
        HackTextRun *uglyHack = static_cast<HackTextRun*>(aTextRun);
        return uglyHack->mRefCnt; }
};

LIBRARY_API MozRefCountType moz2d_text_run_ref_count(gfxTextRun* aTextRun);

#ifdef __cplusplus
}
#endif

#endif /* TEXT_RUN_H_ */
