/* -*- Mode: C++; tab-width: 20; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "gfxFontInfoLoader.h"
#include "nsCRT.h"
#include "gfxPlatformFontList.h"
#include "../2d/Logging.h"              // for gfxCriticalError

using namespace mozilla;

#define LOG_FONTINIT(args) MOZ_LOG(gfxPlatform::GetLog(eGfxLog_fontinit), \
                               LogLevel::Debug, args)
#define LOG_FONTINIT_ENABLED() MOZ_LOG_TEST( \
                                   gfxPlatform::GetLog(eGfxLog_fontinit), \
                                   LogLevel::Debug)

void
FontInfoData::Load()
{
    TimeStamp start = TimeStamp::Now();

    uint32_t i, n = mFontFamiliesToLoad.Length();
    mLoadStats.families = n;
    for (i = 0; i < n && !mCanceled; i++) {
        // font file memory mapping sometimes causes exceptions - bug 1100949
        MOZ_SEH_TRY {
            LoadFontFamilyData(mFontFamiliesToLoad[i]);
        } MOZ_SEH_EXCEPT(EXCEPTION_EXECUTE_HANDLER) {
            gfxCriticalError() <<
                "Exception occurred reading font data for " <<
                NS_ConvertUTF16toUTF8(mFontFamiliesToLoad[i]).get();
        }
    }

    mLoadTime = TimeStamp::Now() - start;
}

class FontInfoLoadCompleteEvent  : public RefCounted<FontInfoLoadCompleteEvent> {
public:
	MOZ_DECLARE_REFCOUNTED_VIRTUAL_TYPENAME(FontInfoLoadCompleteEvent)
	virtual ~FontInfoLoadCompleteEvent() {}


    FontInfoLoadCompleteEvent(FontInfoData *aFontInfo) :
        mFontInfo(aFontInfo)
    {}


    nsresult Run()
    {
        gfxFontInfoLoader *loader =
            static_cast<gfxFontInfoLoader*>(gfxPlatformFontList::PlatformFontList());

        loader->FinalizeLoader(mFontInfo);

        return NS_OK;
    }

    RefPtr<FontInfoData> mFontInfo;
};

class AsyncFontInfoLoader {
public:
	virtual ~AsyncFontInfoLoader() {}

    explicit AsyncFontInfoLoader(FontInfoData *aFontInfo) :
        mFontInfo(aFontInfo)
    {
        mCompleteEvent = new FontInfoLoadCompleteEvent(aFontInfo);
    }

    // runs on separate thread
    nsresult Run()
    {
        // load platform-specific font info
        mFontInfo->Load();

        // post a completion event that transfer the data to the fontlist
        mCompleteEvent->Run();

        return NS_OK;
    }

    RefPtr<FontInfoData> mFontInfo;
    RefPtr<FontInfoLoadCompleteEvent> mCompleteEvent;
};


void
gfxFontInfoLoader::StartLoader(uint32_t aDelay, uint32_t aInterval)
{
    mInterval = 0;

    NS_ASSERTION(!mFontInfo,
                 "fontinfo should be null when starting font loader");

    // sanity check
        if (mState != stateInitial &&
            mState != stateTimerOff &&
            mState != stateTimerOnDelay) {
            CancelLoader();
        }

    mFontInfo = CreateFontInfoData();

    // initialize
    InitLoader();

    // load platform-specific font info
    mFontInfo->Load();

    gfxFontInfoLoader *loader =
            static_cast<gfxFontInfoLoader*>(gfxPlatformFontList::PlatformFontList());

    loader->FinalizeLoader(mFontInfo);
}

void
gfxFontInfoLoader::FinalizeLoader(FontInfoData *aFontInfo)
{
    // Avoid loading data if loader has already been canceled.
    // This should mean that CancelLoader() ran and the Load
    // thread has already Shutdown(), and likely before processing
    // the Shutdown event it handled the load event and sent back
    // our Completion event, thus we end up here.

    if (mFontInfo != aFontInfo) {
        return;
    }

    mLoadTime = mFontInfo->mLoadTime;

    // try to load all font data immediately
    if (LoadFontInfo()) {
        CancelLoader();
        return;
    }
}

void
gfxFontInfoLoader::CancelLoader()
{

    if (mFontInfo) // null during any initial delay
        mFontInfo->mCanceled = true;
    RemoveShutdownObserver();
    CleanupLoader();
}

void
gfxFontInfoLoader::LoadFontInfoTimerFire()
{

    bool done = LoadFontInfo();
    if (done) {
        CancelLoader();
    }
}

gfxFontInfoLoader::~gfxFontInfoLoader()
{
    RemoveShutdownObserver();
    MOZ_COUNT_DTOR(gfxFontInfoLoader);
}

void
gfxFontInfoLoader::AddShutdownObserver()
{

}

void
gfxFontInfoLoader::RemoveShutdownObserver()
{

}
