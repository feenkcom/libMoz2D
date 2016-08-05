/* -*- Mode: C++; tab-width: 20; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/layers/CompositorBridgeChild.h"
#include "mozilla/layers/CompositorThread.h"
#include "mozilla/layers/ImageBridgeChild.h"
#include "mozilla/layers/SharedBufferManagerChild.h"
#include "mozilla/layers/ISurfaceAllocator.h"     // for GfxMemoryImageReporter
#include "mozilla/gfx/GPUProcessManager.h"
#include "mozilla/ClearOnShutdown.h"
#include "mozilla/Telemetry.h"
#include "mozilla/TimeStamp.h"

#include "mozilla/Logging.h"
#include "mozilla/Services.h"
#include "prprf.h"

#include "gfxCrashReporterUtils.h"
#include "gfxPlatform.h"
#include "gfxPrefs.h"
#include "gfxEnv.h"
#include "gfxTextRun.h"
#include "gfxConfig.h"
#include "MediaPrefs.h"

#ifdef XP_WIN
#include <process.h>
#define getpid _getpid
#else
#include <unistd.h>
#endif

#include "nsXULAppAPI.h"
#include "nsDirectoryServiceUtils.h"
#include "nsDirectoryServiceDefs.h"

#if defined(XP_WIN)
#include "gfxWindowsPlatform.h"
#elif defined(XP_MACOSX)
#include "gfxPlatformMac.h"
#include "gfxQuartzSurface.h"
#elif defined(MOZ_WIDGET_GTK)
#include "gfxPlatformGtk.h"
#elif defined(ANDROID)
#include "gfxAndroidPlatform.h"
#endif

#ifdef XP_WIN
#include "mozilla/WindowsVersion.h"
#endif

#include "nsGkAtoms.h"
#include "gfxPlatformFontList.h"
#include "gfxContext.h"
#include "gfxImageSurface.h"
#include "nsUnicodeProperties.h"
#include "harfbuzz/hb.h"
#include "gfxGraphiteShaper.h"
#include "gfx2DGlue.h"
#include "gfxGradientCache.h"
#include "gfxUtils.h" // for NextPowerOfTwo

#include "nsUnicodeRange.h"
#include "nsServiceManagerUtils.h"
#include "nsTArray.h"
#include "nsILocaleService.h"
#include "nsIObserverService.h"
#include "nsIScreenManager.h"
#include "FrameMetrics.h"
#include "MainThreadUtils.h"
#ifdef MOZ_CRASHREPORTER
#include "nsExceptionHandler.h"
#endif

#include "nsWeakReference.h"

#include "cairo.h"
#include "qcms.h"

#include "imgITools.h"

#include "plstr.h"
#include "nsCRT.h"
#include "GLContext.h"
#include "GLContextProvider.h"
#include "mozilla/gfx/Logging.h"

#if defined(MOZ_WIDGET_GTK)
#include "gfxPlatformGtk.h" // xxx - for UseFcFontList
#endif

#ifdef MOZ_WIDGET_ANDROID
#include "TexturePoolOGL.h"
#endif

#ifdef MOZ_WIDGET_GONK
#include "mozilla/layers/GrallocTextureHost.h"
#endif

#include "mozilla/Hal.h"

#ifdef USE_SKIA
# ifdef __GNUC__
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wshadow"
# endif
# include "skia/include/core/SkGraphics.h"
# ifdef USE_SKIA_GPU
#  include "skia/include/gpu/GrContext.h"
#  include "skia/include/gpu/gl/GrGLInterface.h"
#  include "SkiaGLGlue.h"
# endif
# ifdef MOZ_ENABLE_FREETYPE
#  include "skia/include/ports/SkTypeface_cairo.h"
# endif
# ifdef __GNUC__
#  pragma GCC diagnostic pop // -Wshadow
# endif
static const uint32_t kDefaultGlyphCacheSize = -1;

#endif

#if !defined(USE_SKIA) || !defined(USE_SKIA_GPU)
class mozilla::gl::SkiaGLGlue : public GenericAtomicRefCounted {
};
#endif

#include "mozilla/Preferences.h"
#include "mozilla/Assertions.h"
#include "mozilla/Atomics.h"
#include "mozilla/Attributes.h"
#include "mozilla/Mutex.h"

#include "nsAlgorithm.h"
#include "nsIGfxInfo.h"
#include "nsIXULRuntime.h"
#include "VsyncSource.h"
#include "SoftwareVsyncSource.h"
#include "nscore.h" // for NS_FREE_PERMANENT_DATA
#include "mozilla/dom/ContentChild.h"
#include "gfxVR.h"
#include "VRManagerChild.h"
#include "mozilla/gfx/GPUParent.h"

namespace mozilla {
namespace layers {
#ifdef MOZ_WIDGET_GONK
void InitGralloc();
#endif
} // namespace layers
} // namespace mozilla

using namespace mozilla;
using namespace mozilla::layers;
using namespace mozilla::gl;
using namespace mozilla::gfx;

gfxPlatform *gPlatform = nullptr;
static bool gEverInitialized = false;

static Mutex* gGfxPlatformPrefsLock = nullptr;

// These two may point to the same profile
static qcms_profile *gCMSOutputProfile = nullptr;
static qcms_profile *gCMSsRGBProfile = nullptr;

static qcms_transform *gCMSRGBTransform = nullptr;
static qcms_transform *gCMSInverseRGBTransform = nullptr;
static qcms_transform *gCMSRGBATransform = nullptr;

static bool gCMSInitialized = false;
static eCMSMode gCMSMode = eCMSMode_Off;


static void ShutdownCMS();

#include "mozilla/gfx/2D.h"
#include "mozilla/gfx/SourceSurfaceCairo.h"
using namespace mozilla::gfx;


#define GFX_DOWNLOADABLE_FONTS_ENABLED "gfx.downloadable_fonts.enabled"

#define GFX_PREF_FALLBACK_USE_CMAPS  "gfx.font_rendering.fallback.always_use_cmaps"

#define GFX_PREF_OPENTYPE_SVG "gfx.font_rendering.opentype_svg.enabled"

#define GFX_PREF_WORD_CACHE_CHARLIMIT "gfx.font_rendering.wordcache.charlimit"
#define GFX_PREF_WORD_CACHE_MAXENTRIES "gfx.font_rendering.wordcache.maxentries"

#define GFX_PREF_GRAPHITE_SHAPING "gfx.font_rendering.graphite.enabled"

#define BIDI_NUMERAL_PREF "bidi.numeral"

#define GFX_PREF_CMS_FORCE_SRGB "gfx.color_management.force_srgb"

gfxPlatform::gfxPlatform()
: mTileWidth(-1)
, mTileHeight(-1)
, mCompositorBackend(layers::LayersBackend::LAYERS_NONE)
, mScreenDepth(0)
, mDeviceCounter(0)
{
	mAllowDownloadableFonts = UNINITIALIZED_VALUE;
	mFallbackUsesCmaps = UNINITIALIZED_VALUE;

	mWordCacheCharLimit = UNINITIALIZED_VALUE;
	mWordCacheMaxEntries = UNINITIALIZED_VALUE;
	mGraphiteShapingEnabled = UNINITIALIZED_VALUE;
	mOpenTypeSVGEnabled = UNINITIALIZED_VALUE;
	mBidiNumeralOption = UNINITIALIZED_VALUE;

	mSkiaGlue = nullptr;

	uint32_t canvasMask = BackendTypeBit(BackendType::CAIRO) | BackendTypeBit(BackendType::SKIA);
	uint32_t contentMask = BackendTypeBit(BackendType::CAIRO);
	InitBackendPrefs(canvasMask, BackendType::CAIRO,
			contentMask, BackendType::CAIRO);

}

void gfxPlatform::PurgeCache(){
	Factory::PurgeAllCaches();
	gfxGradientCache::PurgeAllCaches();

	gfxPlatform::PurgeSkiaFontCache();
	gfxPlatform::GetPlatform()->PurgeSkiaGPUCache();
}

gfxPlatform*
gfxPlatform::GetPlatform()
{
	if (!gPlatform) {
		Init();
	}
	return gPlatform;
}

bool
gfxPlatform::Initialized()
{
	return !!gPlatform;
}

#if defined(USE_SKIA)
static uint32_t GetSkiaGlyphCacheSize()
{
	// Only increase font cache size on non-android to save memory.
#if !defined(MOZ_WIDGET_ANDROID)
	// 10mb as the default cache size on desktop due to talos perf tweaking.
	// Chromium uses 20mb and skia default uses 2mb.
	// We don't need to change the font cache count since we usually
	// cache thrash due to asian character sets in talos.
	// Only increase memory on the content proces
	uint32_t cacheSize = 10 * 1024 * 1024;

	return cacheSize;
#else
	return kDefaultGlyphCacheSize;
#endif // MOZ_WIDGET_ANDROID
}
#endif

void
gfxPlatform::Init()
{
	if (gEverInitialized) {
		NS_RUNTIMEABORT("Already started???");
	}
	gEverInitialized = true;

	// Initialize the preferences by creating the singleton.
	gfxPrefs::GetSingleton();

	mozilla::gfx::Config cfg;
	cfg.mMaxTextureSize = gfxPrefs::MaxTextureSize();
	cfg.mMaxAllocSize = gfxPrefs::MaxAllocSize();

	gfx::Factory::Init(cfg);

	gGfxPlatformPrefsLock = new Mutex("gfxPlatform::gGfxPlatformPrefsLock");

#if defined(XP_WIN)
	gPlatform = new gfxWindowsPlatform;
#elif defined(XP_MACOSX)
	gPlatform = new gfxPlatformMac;
#elif defined(MOZ_WIDGET_GTK)
	gPlatform = new gfxPlatformGtk;
#elif defined(ANDROID)
	gPlatform = new gfxAndroidPlatform;
#else
#error "No gfxPlatform implementation available"
#endif

#ifdef USE_SKIA
	SkGraphics::Init();
#  ifdef MOZ_ENABLE_FREETYPE
	SkInitCairoFT(gPlatform->FontHintingEnabled());
#  endif
#endif

#ifdef MOZ_GL_DEBUG
	GLContext::StaticInit();
#endif


	gPlatform->PopulateScreenInfo();

	gPlatform->ComputeTileSize();

	nsresult rv;

	bool usePlatformFontList = true;
#if defined(MOZ_WIDGET_GTK)
	usePlatformFontList = gfxPlatformGtk::UseFcFontList();
#endif

	if (usePlatformFontList) {
		rv = gfxPlatformFontList::Init();
		if (NS_FAILED(rv)) {
			NS_RUNTIMEABORT("Could not initialize gfxPlatformFontList");
		}
	}

	gPlatform->mScreenReferenceSurface =
			gPlatform->CreateOffscreenSurface(IntSize(1, 1),
					SurfaceFormat::A8R8G8B8_UINT32);
	if (!gPlatform->mScreenReferenceSurface) {
		NS_RUNTIMEABORT("Could not initialize mScreenReferenceSurface");
	}

	gPlatform->mScreenReferenceDrawTarget =
			gPlatform->CreateOffscreenContentDrawTarget(IntSize(1, 1),
					SurfaceFormat::B8G8R8A8);
	if (!gPlatform->mScreenReferenceDrawTarget ||
			!gPlatform->mScreenReferenceDrawTarget->IsValid()) {
		NS_RUNTIMEABORT("Could not initialize mScreenReferenceDrawTarget");
	}

	rv = gfxFontCache::Init();
	if (NS_FAILED(rv)) {
		NS_RUNTIMEABORT("Could not initialize gfxFontCache");
	}

#ifdef MOZ_WIDGET_ANDROID
	// Texture pool init
	TexturePoolOGL::Init();
#endif

#ifdef MOZ_WIDGET_GONK
	mozilla::layers::InitGralloc();
#endif

	CreateCMSOutputProfile();

#ifdef USE_SKIA
	uint32_t skiaCacheSize = GetSkiaGlyphCacheSize();
	if (skiaCacheSize != kDefaultGlyphCacheSize) {
		SkGraphics::SetFontCacheLimit(skiaCacheSize);
	}
#endif

}

void
gfxPlatform::InitOpenGLConfig()
{

}

void
gfxPlatform::InitAcceleration()
{

}

bool
gfxPlatform::AccelerateLayersByDefault()
{
	return false;
}

bool
gfxPlatform::UpdateDeviceInitData()
{
	return false;
}

bool
gfxPlatform::CanUseHardwareVideoDecoding()
{
	return false;
}

bool
gfxPlatform::UsesOffMainThreadCompositing()
{
	return false;
}

void
gfxPlatform::InitLayersIPC()
{

}
void
gfxPlatform::ShutdownLayersIPC()
{

}

int
gfxPlatform::GetSoftwareVsyncRate()
{
	return GetDefaultFrameRate();
}

bool
gfxPlatform::ForceSoftwareVsync()
{
	return false;
}

int
gfxPlatform::GetDefaultFrameRate()
{
	return 60;
}

/***
 * The preference "layout.frame_rate" has 3 meanings depending on the value:
 *
 * -1 = Auto (default), use hardware vsync or software vsync @ 60 hz if hw vsync fails.
 *  0 = ASAP mode - used during talos testing.
 *  X = Software vsync at a rate of X times per second.
 */
already_AddRefed<mozilla::gfx::VsyncSource>
gfxPlatform::CreateHardwareVsyncSource()
{
  return nullptr;
}

void
gfxPlatform::Shutdown()
{
	// In some cases, gPlatform may not be created but Shutdown() called,
	// e.g., during xpcshell tests.
	if (!gPlatform) {
		return;
	}

	MOZ_ASSERT(!sLayersIPCIsUp);

	// These may be called before the corresponding subsystems have actually
	// started up. That's OK, they can handle it.
	gfxFontCache::Shutdown();
	gfxFontGroup::Shutdown();
	gfxGradientCache::Shutdown();
	gfxAlphaBoxBlur::ShutdownBlurCache();
	gfxGraphiteShaper::Shutdown();
	gfxPlatformFontList::Shutdown();

	// Free the various non-null transforms and loaded profiles
	ShutdownCMS();


	gPlatform->mSkiaGlue = nullptr;

#ifdef MOZ_WIDGET_ANDROID
	// Shut down the texture pool
	TexturePoolOGL::Shutdown();
#endif

#if defined(XP_WIN)
	// The above shutdown calls operate on the available context providers on
	// most platforms.  Windows is a "special snowflake", though, and has three
	// context providers available, so we have to shut all of them down.
	// We should only support the default GL provider on Windows; then, this
	// could go away. Unfortunately, we currently support WGL (the default) for
	// WebGL on Optimus.
	GLContextProviderEGL::Shutdown();
#endif

	gfx::Factory::ShutDown();

	delete gGfxPlatformPrefsLock;

	gfxPrefs::DestroySingleton();
	gfxFont::DestroySingletons();

	delete gPlatform;
	gPlatform = nullptr;
}

gfxPlatform::~gfxPlatform()
{
	mScreenReferenceSurface = nullptr;
	mScreenReferenceDrawTarget = nullptr;

	// The cairo folks think we should only clean up in debug builds,
	// but we're generally in the habit of trying to shut down as
	// cleanly as possible even in production code, so call this
	// cairo_debug_* function unconditionally.
	//
	// because cairo can assert and thus crash on shutdown, don't do this in release builds
#ifdef NS_FREE_PERMANENT_DATA
#ifdef USE_SKIA
	// must do Skia cleanup before Cairo cleanup, because Skia may be referencing
	// Cairo objects e.g. through SkCairoFTTypeface
	SkGraphics::PurgeFontCache();
#endif

#if MOZ_TREE_CAIRO
	cairo_debug_reset_static_data();
#endif
#endif
}

already_AddRefed<DrawTarget>
gfxPlatform::CreateDrawTargetForSurface(gfxASurface *aSurface, const IntSize& aSize)
{
	SurfaceFormat format = aSurface->GetSurfaceFormat();
	RefPtr<DrawTarget> drawTarget = Factory::CreateDrawTargetForCairoSurface(aSurface->CairoSurface(), aSize, &format);
	if (!drawTarget) {
		gfxWarning() << "gfxPlatform::CreateDrawTargetForSurface failed in CreateDrawTargetForCairoSurface";
		return nullptr;
	}
	return drawTarget.forget();
}

cairo_user_data_key_t kSourceSurface;

/**
 * Record the backend that was used to construct the SourceSurface.
 * When getting the cached SourceSurface for a gfxASurface/DrawTarget pair,
 * we check to make sure the DrawTarget's backend matches the backend
 * for the cached SourceSurface, and only use it if they match. This
 * can avoid expensive and unnecessary readbacks.
 */
struct SourceSurfaceUserData
{
	RefPtr<SourceSurface> mSrcSurface;
	BackendType mBackendType;
};

void SourceBufferDestroy(void *srcSurfUD)
{
	delete static_cast<SourceSurfaceUserData*>(srcSurfUD);
}

UserDataKey kThebesSurface;

struct DependentSourceSurfaceUserData
{
	RefPtr<gfxASurface> mSurface;
};

void SourceSurfaceDestroyed(void *aData)
{
	delete static_cast<DependentSourceSurfaceUserData*>(aData);
}

void
gfxPlatform::ClearSourceSurfaceForSurface(gfxASurface *aSurface)
{
	aSurface->SetData(&kSourceSurface, nullptr, nullptr);
}

/* static */ already_AddRefed<SourceSurface>
gfxPlatform::GetSourceSurfaceForSurface(DrawTarget *aTarget, gfxASurface *aSurface)
{
	if (!aSurface->CairoSurface() || aSurface->CairoStatus()) {
		return nullptr;
	}

	if (!aTarget) {
		aTarget = gfxPlatform::GetPlatform()->ScreenReferenceDrawTarget();
	}

	void *userData = aSurface->GetData(&kSourceSurface);

	if (userData) {
		SourceSurfaceUserData *surf = static_cast<SourceSurfaceUserData*>(userData);

		if (surf->mSrcSurface->IsValid() && surf->mBackendType == aTarget->GetBackendType()) {
			RefPtr<SourceSurface> srcSurface(surf->mSrcSurface);
			return srcSurface.forget();
		}
		// We can just continue here as when setting new user data the destroy
		// function will be called for the old user data.
	}

	SurfaceFormat format = aSurface->GetSurfaceFormat();

	if (aTarget->GetBackendType() == BackendType::CAIRO) {
		// If we're going to be used with a CAIRO DrawTarget, then just create a
		// SourceSurfaceCairo since we don't know the underlying type of the CAIRO
		// DrawTarget and can't pick a better surface type. Doing this also avoids
		// readback of aSurface's surface into memory if, for example, aSurface
		// wraps an xlib cairo surface (which can be important to avoid a major
		// slowdown).
		//
		// We return here regardless of whether CreateSourceSurfaceFromNativeSurface
		// succeeds or not since we don't expect to be able to do any better below
		// if it fails.
		//
		// Note that the returned SourceSurfaceCairo holds a strong reference to
		// the cairo_surface_t* that it wraps, which essencially means it holds a
		// strong reference to aSurface since aSurface shares its
		// cairo_surface_t*'s reference count variable. As a result we can't cache
		// srcBuffer on aSurface (see below) since aSurface would then hold a
		// strong reference back to srcBuffer, creating a reference loop and a
		// memory leak. Not caching is fine since wrapping is cheap enough (no
		// copying) so we can just wrap again next time we're called.
		return Factory::CreateSourceSurfaceForCairoSurface(aSurface->CairoSurface(),
				aSurface->GetSize(), format);
	}

	RefPtr<SourceSurface> srcBuffer;

	// Currently no other DrawTarget types implement CreateSourceSurfaceFromNativeSurface

	if (!srcBuffer) {
		// If aSurface wraps data, we can create a SourceSurfaceRawData that wraps
		// the same data, then optimize it for aTarget:
		RefPtr<DataSourceSurface> surf = GetWrappedDataSourceSurface(aSurface);
		if (surf) {
			srcBuffer = aTarget->OptimizeSourceSurface(surf);
			if (srcBuffer == surf) {
				// GetWrappedDataSourceSurface returns a SourceSurface that holds a
				// strong reference to aSurface since it wraps aSurface's data and
				// needs it to stay alive. As a result we can't cache srcBuffer on
				// aSurface (below) since aSurface would then hold a strong reference
				// back to srcBuffer, creating a reference loop and a memory leak. Not
				// caching is fine since wrapping is cheap enough (no copying) so we
				// can just wrap again next time we're called.
				//
				// Note that the check below doesn't catch this since srcBuffer will be a
				// SourceSurfaceRawData object (even if aSurface is not a gfxImageSurface
				// object), which is why we need this separate check.
				return srcBuffer.forget();
			}
		}
	}

	if (!srcBuffer) {
		MOZ_ASSERT(aTarget->GetBackendType() != BackendType::CAIRO,
				"We already tried CreateSourceSurfaceFromNativeSurface with a "
				"DrawTargetCairo above");
		// We've run out of performant options. We now try creating a SourceSurface
		// using a temporary DrawTargetCairo and then optimizing it to aTarget's
		// actual type. The CreateSourceSurfaceFromNativeSurface() call will
		// likely create a DataSourceSurface (possibly involving copying and/or
		// readback), and the OptimizeSourceSurface may well copy again and upload
		// to the GPU. So, while this code path is rarely hit, hitting it may be
		// very slow.
		srcBuffer = Factory::CreateSourceSurfaceForCairoSurface(aSurface->CairoSurface(),
				aSurface->GetSize(), format);
		if (srcBuffer) {
			srcBuffer = aTarget->OptimizeSourceSurface(srcBuffer);
		}
	}

	if (!srcBuffer) {
		return nullptr;
	}

	if ((srcBuffer->GetType() == SurfaceType::CAIRO &&
			static_cast<SourceSurfaceCairo*>(srcBuffer.get())->GetSurface() ==
					aSurface->CairoSurface()) ||
			(srcBuffer->GetType() == SurfaceType::CAIRO_IMAGE &&
					static_cast<DataSourceSurfaceCairo*>(srcBuffer.get())->GetSurface() ==
							aSurface->CairoSurface())) {
		// See the "Note that the returned SourceSurfaceCairo..." comment above.
		return srcBuffer.forget();
	}

	// Add user data to aSurface so we can cache lookups in the future.
	SourceSurfaceUserData *srcSurfUD = new SourceSurfaceUserData;
	srcSurfUD->mBackendType = aTarget->GetBackendType();
	srcSurfUD->mSrcSurface = srcBuffer;
	aSurface->SetData(&kSourceSurface, srcSurfUD, SourceBufferDestroy);

	return srcBuffer.forget();
}

already_AddRefed<DataSourceSurface>
gfxPlatform::GetWrappedDataSourceSurface(gfxASurface* aSurface)
{
	RefPtr<gfxImageSurface> image = aSurface->GetAsImageSurface();
	if (!image) {
		return nullptr;
	}
	RefPtr<DataSourceSurface> result =
			Factory::CreateWrappingDataSourceSurface(image->Data(),
					image->Stride(),
					image->GetSize(),
					ImageFormatToSurfaceFormat(image->Format()));

	if (!result) {
		return nullptr;
	}

	// If we wrapped the underlying data of aSurface, then we need to add user data
	// to make sure aSurface stays alive until we are done with the data.
	DependentSourceSurfaceUserData *srcSurfUD = new DependentSourceSurfaceUserData;
	srcSurfUD->mSurface = aSurface;
	result->AddUserData(&kThebesSurface, srcSurfUD, SourceSurfaceDestroyed);

	return result.forget();
}

already_AddRefed<ScaledFont>
gfxPlatform::GetScaledFontForFont(DrawTarget* aTarget, gfxFont *aFont)
{
	NativeFont nativeFont;
	nativeFont.mType = NativeFontType::CAIRO_FONT_FACE;
	nativeFont.mFont = aFont->GetCairoScaledFont();
	return Factory::CreateScaledFontForNativeFont(nativeFont,
			aFont->GetAdjustedSize());
}

int
gfxPlatform::GetTileWidth()
{
	MOZ_ASSERT(mTileWidth != -1);
	return mTileWidth;
}

int
gfxPlatform::GetTileHeight()
{
	MOZ_ASSERT(mTileHeight != -1);
	return mTileHeight;
}

void
gfxPlatform::SetTileSize(int aWidth, int aHeight)
{
	// Don't allow changing the tile size after we've set it.
	// Right now the code assumes that the tile size doesn't change.
	MOZ_ASSERT((mTileWidth == -1 && mTileHeight == -1) ||
			(mTileWidth == aWidth && mTileHeight == aHeight));

	mTileWidth = aWidth;
	mTileHeight = aHeight;
}

void
gfxPlatform::ComputeTileSize()
{

	int32_t w = gfxPrefs::LayersTileWidth();
	int32_t h = gfxPrefs::LayersTileHeight();

	if (gfxPrefs::LayersTilesAdjust()) {
		gfx::IntSize screenSize = GetScreenSize();
		if (screenSize.width > 0) {
			// Choose a size so that there are between 2 and 4 tiles per screen width.
			// FIXME: we should probably make sure this is within the max texture size,
			// but I think everything should at least support 1024
			w = h = clamped(int32_t(RoundUpPow2(screenSize.width)) / 4, 256, 1024);
		}

#ifdef MOZ_WIDGET_GONK
		android::sp<android::GraphicBuffer> alloc =
				new android::GraphicBuffer(w, h, android::PIXEL_FORMAT_RGBA_8888,
						android::GraphicBuffer::USAGE_SW_READ_OFTEN |
						android::GraphicBuffer::USAGE_SW_WRITE_OFTEN |
						android::GraphicBuffer::USAGE_HW_TEXTURE);

		if (alloc.get()) {
			w = alloc->getStride(); // We want the tiles to be gralloc stride aligned.
		}
#endif
	}

	SetTileSize(w, h);
}

void
gfxPlatform::PopulateScreenInfo()
{

}

bool
gfxPlatform::SupportsAzureContentForDrawTarget(DrawTarget* aTarget)
{
	if (!aTarget || !aTarget->IsValid()) {
		return false;
	}

#ifdef USE_SKIA_GPU
	// Skia content rendering doesn't support GPU acceleration, so we can't
	// use the same backend if the current backend is accelerated.
	if ((aTarget->GetType() == DrawTargetType::HARDWARE_RASTER)
			&& (aTarget->GetBackendType() ==  BackendType::SKIA))
	{
		return false;
	}
#endif

	return SupportsAzureContentForType(aTarget->GetBackendType());
}

bool gfxPlatform::UseAcceleratedCanvas()
{
	// Allow acceleration on Skia if the preference is set, unless it's blocked
	if (mPreferredCanvasBackend == BackendType::SKIA && gfxPrefs::CanvasAzureAccelerated()) {
		nsCOMPtr<nsIGfxInfo> gfxInfo = do_GetService("@mozilla.org/gfx/info;1");
		int32_t status;
		nsCString discardFailureId;
		return !gfxInfo ||
				(NS_SUCCEEDED(gfxInfo->GetFeatureStatus(nsIGfxInfo::FEATURE_CANVAS2D_ACCELERATION,
						discardFailureId,
						&status)) &&
						status == nsIGfxInfo::FEATURE_STATUS_OK);
	}
	return false;
}

void
gfxPlatform::InitializeSkiaCacheLimits()
{
	if (UseAcceleratedCanvas()) {
#ifdef USE_SKIA_GPU
		bool usingDynamicCache = gfxPrefs::CanvasSkiaGLDynamicCache();
		int cacheItemLimit = gfxPrefs::CanvasSkiaGLCacheItems();
		int cacheSizeLimit = gfxPrefs::CanvasSkiaGLCacheSize();

		// Prefs are in megabytes, but we want the sizes in bytes
		cacheSizeLimit *= 1024*1024;

		if (usingDynamicCache) {
			if (mTotalSystemMemory < 512*1024*1024) {
				// We need a very minimal cache on anything smaller than 512mb.
				// Note the large jump as we cross 512mb (from 2mb to 32mb).
				cacheSizeLimit = 2*1024*1024;
			} else if (mTotalSystemMemory > 0) {
				cacheSizeLimit = mTotalSystemMemory / 16;
			}
		}

#ifdef DEBUG
		printf_stderr("Determined SkiaGL cache limits: Size %i, Items: %i\n", cacheSizeLimit, cacheItemLimit);
#endif

		mSkiaGlue->GetGrContext()->setResourceCacheLimits(cacheItemLimit, cacheSizeLimit);
#endif
	}
}

void
gfxPlatform::PurgeSkiaFontCache()
{
#ifdef USE_SKIA
	if (gfxPlatform::GetPlatform()->GetDefaultContentBackend() == BackendType::SKIA) {
		SkGraphics::PurgeFontCache();
	}
#endif
}

void
gfxPlatform::PurgeSkiaGPUCache()
{
#ifdef USE_SKIA_GPU
	if (!mSkiaGlue)
		return;

	mSkiaGlue->GetGrContext()->freeGpuResources();
	// GrContext::flush() doesn't call glFlush. Call it here.
	mSkiaGlue->GetGLContext()->MakeCurrent();
	mSkiaGlue->GetGLContext()->fFlush();
#endif
}

bool
gfxPlatform::HasEnoughTotalSystemMemoryForSkiaGL()
{
#ifdef MOZ_WIDGET_GONK
	if (mTotalSystemMemory < 250*1024*1024) {
		return false;
	}
#endif
	return true;
}

already_AddRefed<DrawTarget>
gfxPlatform::CreateDrawTargetForBackend(BackendType aBackend, const IntSize& aSize, SurfaceFormat aFormat)
{
	// There is a bunch of knowledge in the gfxPlatform heirarchy about how to
	// create the best offscreen surface for the current system and situation. We
	// can easily take advantage of this for the Cairo backend, so that's what we
	// do.
	// mozilla::gfx::Factory can get away without having all this knowledge for
	// now, but this might need to change in the future (using
	// CreateOffscreenSurface() and CreateDrawTargetForSurface() for all
	// backends).
	if (aBackend == BackendType::CAIRO) {
		RefPtr<gfxASurface> surf = CreateOffscreenSurface(aSize, SurfaceFormatToImageFormat(aFormat));
		if (!surf || surf->CairoStatus()) {
			return nullptr;
		}

		return CreateDrawTargetForSurface(surf, aSize);
	} else {
		return Factory::CreateDrawTarget(aBackend, aSize, aFormat);
	}
}

already_AddRefed<DrawTarget>
gfxPlatform::CreateOffscreenCanvasDrawTarget(const IntSize& aSize, SurfaceFormat aFormat)
{
	NS_ASSERTION(mPreferredCanvasBackend != BackendType::NONE, "No backend.");
	RefPtr<DrawTarget> target = CreateDrawTargetForBackend(mPreferredCanvasBackend, aSize, aFormat);
	if (target ||
			mFallbackCanvasBackend == BackendType::NONE) {
		return target.forget();
	}

#ifdef XP_WIN
	// On Windows, the fallback backend (Cairo) should use its image backend.
	return Factory::CreateDrawTarget(mFallbackCanvasBackend, aSize, aFormat);
#else
	return CreateDrawTargetForBackend(mFallbackCanvasBackend, aSize, aFormat);
#endif
}

already_AddRefed<DrawTarget>
gfxPlatform::CreateOffscreenContentDrawTarget(const IntSize& aSize, SurfaceFormat aFormat)
{
	NS_ASSERTION(mPreferredCanvasBackend != BackendType::NONE, "No backend.");
	return CreateDrawTargetForBackend(mContentBackend, aSize, aFormat);
}

already_AddRefed<DrawTarget>
gfxPlatform::CreateDrawTargetForData(unsigned char* aData, const IntSize& aSize, int32_t aStride, SurfaceFormat aFormat)
{
	NS_ASSERTION(mContentBackend != BackendType::NONE, "No backend.");

	BackendType backendType = mContentBackend;

	if (!Factory::DoesBackendSupportDataDrawtarget(mContentBackend)) {
		backendType = BackendType::CAIRO;
	}

	RefPtr<DrawTarget> dt = Factory::CreateDrawTargetForData(backendType,
			aData, aSize,
			aStride, aFormat);

	return dt.forget();
}

/* static */ BackendType
gfxPlatform::BackendTypeForName(const nsCString& aName)
{
	if (aName.EqualsLiteral("cairo"))
		return BackendType::CAIRO;
	if (aName.EqualsLiteral("skia"))
		return BackendType::SKIA;
	if (aName.EqualsLiteral("direct2d"))
		return BackendType::DIRECT2D;
	if (aName.EqualsLiteral("direct2d1.1"))
		return BackendType::DIRECT2D1_1;
	if (aName.EqualsLiteral("cg"))
		return BackendType::COREGRAPHICS;
	return BackendType::NONE;
}

nsresult
gfxPlatform::GetFontList(nsIAtom *aLangGroup,
		const nsACString& aGenericFamily,
		nsTArray<nsString>& aListOfFonts)
{
	gfxPlatformFontList::PlatformFontList()->GetFontList(aLangGroup,
			aGenericFamily,
			aListOfFonts);
	return NS_OK;
}

nsresult
gfxPlatform::UpdateFontList()
{
	gfxPlatformFontList::PlatformFontList()->UpdateFontList();
	return NS_OK;
}

nsresult
gfxPlatform::GetStandardFamilyName(const nsAString& aFontName,
		nsAString& aFamilyName)
{
	gfxPlatformFontList::PlatformFontList()->GetStandardFamilyName(aFontName,
			aFamilyName);
	return NS_OK;
}

bool
gfxPlatform::DownloadableFontsEnabled()
{
	if (mAllowDownloadableFonts == UNINITIALIZED_VALUE) {
		mAllowDownloadableFonts =
				Preferences::GetBool(GFX_DOWNLOADABLE_FONTS_ENABLED, false);
	}

	return mAllowDownloadableFonts;
}

bool
gfxPlatform::UseCmapsDuringSystemFallback()
{
	if (mFallbackUsesCmaps == UNINITIALIZED_VALUE) {
		mFallbackUsesCmaps =
				Preferences::GetBool(GFX_PREF_FALLBACK_USE_CMAPS, false);
	}

	return mFallbackUsesCmaps;
}

bool
gfxPlatform::OpenTypeSVGEnabled()
{
	if (mOpenTypeSVGEnabled == UNINITIALIZED_VALUE) {
		mOpenTypeSVGEnabled =
				Preferences::GetBool(GFX_PREF_OPENTYPE_SVG, false);
	}

	return mOpenTypeSVGEnabled > 0;
}

uint32_t
gfxPlatform::WordCacheCharLimit()
{
	if (mWordCacheCharLimit == UNINITIALIZED_VALUE) {
		mWordCacheCharLimit =
				Preferences::GetInt(GFX_PREF_WORD_CACHE_CHARLIMIT, 32);
		if (mWordCacheCharLimit < 0) {
			mWordCacheCharLimit = 32;
		}
	}

	return uint32_t(mWordCacheCharLimit);
}

uint32_t
gfxPlatform::WordCacheMaxEntries()
{
	if (mWordCacheMaxEntries == UNINITIALIZED_VALUE) {
		mWordCacheMaxEntries =
				Preferences::GetInt(GFX_PREF_WORD_CACHE_MAXENTRIES, 10000);
		if (mWordCacheMaxEntries < 0) {
			mWordCacheMaxEntries = 10000;
		}
	}

	return uint32_t(mWordCacheMaxEntries);
}

bool
gfxPlatform::UseGraphiteShaping()
{
	if (mGraphiteShapingEnabled == UNINITIALIZED_VALUE) {
		mGraphiteShapingEnabled =
				Preferences::GetBool(GFX_PREF_GRAPHITE_SHAPING, false);
	}

	return mGraphiteShapingEnabled;
}

gfxFontEntry*
gfxPlatform::LookupLocalFont(const nsAString& aFontName,
		uint16_t aWeight,
		int16_t aStretch,
		uint8_t aStyle)
{
	return gfxPlatformFontList::PlatformFontList()->LookupLocalFont(aFontName,
			aWeight,
			aStretch,
			aStyle);
}

gfxFontEntry*
gfxPlatform::MakePlatformFont(const nsAString& aFontName,
		uint16_t aWeight,
		int16_t aStretch,
		uint8_t aStyle,
		const uint8_t* aFontData,
		uint32_t aLength)
{
	return gfxPlatformFontList::PlatformFontList()->MakePlatformFont(aFontName,
			aWeight,
			aStretch,
			aStyle,
			aFontData,
			aLength);
}

mozilla::layers::DiagnosticTypes
gfxPlatform::GetLayerDiagnosticTypes()
{
	mozilla::layers::DiagnosticTypes type = DiagnosticTypes::NO_DIAGNOSTIC;
	if (gfxPrefs::DrawLayerBorders()) {
		type |= mozilla::layers::DiagnosticTypes::LAYER_BORDERS;
	}
	if (gfxPrefs::DrawTileBorders()) {
		type |= mozilla::layers::DiagnosticTypes::TILE_BORDERS;
	}
	if (gfxPrefs::DrawBigImageBorders()) {
		type |= mozilla::layers::DiagnosticTypes::BIGIMAGE_BORDERS;
	}
	if (gfxPrefs::FlashLayerBorders()) {
		type |= mozilla::layers::DiagnosticTypes::FLASH_BORDERS;
	}
	return type;
}

void
gfxPlatform::InitBackendPrefs(uint32_t aCanvasBitmask, BackendType aCanvasDefault,
		uint32_t aContentBitmask, BackendType aContentDefault)
{
	mPreferredCanvasBackend = GetCanvasBackendPref(aCanvasBitmask);
	if (mPreferredCanvasBackend == BackendType::NONE) {
		mPreferredCanvasBackend = aCanvasDefault;
	}

	if (mPreferredCanvasBackend == BackendType::DIRECT2D1_1) {
		// Falling back to D2D 1.0 won't help us here. When D2D 1.1 DT creation
		// fails it means the surface was too big or there's something wrong with
		// the device. D2D 1.0 will encounter a similar situation.
		mFallbackCanvasBackend =
				GetCanvasBackendPref(aCanvasBitmask &
						~(BackendTypeBit(mPreferredCanvasBackend) | BackendTypeBit(BackendType::DIRECT2D)));
	} else {
		mFallbackCanvasBackend =
				GetCanvasBackendPref(aCanvasBitmask & ~BackendTypeBit(mPreferredCanvasBackend));
	}

	mContentBackendBitmask = aContentBitmask;
	mContentBackend = GetContentBackendPref(mContentBackendBitmask);
	if (mContentBackend == BackendType::NONE) {
		mContentBackend = aContentDefault;
		// mContentBackendBitmask is our canonical reference for supported
		// backends so we need to add the default if we are using it and
				// overriding the prefs.
				mContentBackendBitmask |= BackendTypeBit(aContentDefault);
	}
}

/* static */ BackendType
gfxPlatform::GetCanvasBackendPref(uint32_t aBackendBitmask)
{
	return GetBackendPref("gfx.canvas.azure.backends", aBackendBitmask);
}

/* static */ BackendType
gfxPlatform::GetContentBackendPref(uint32_t &aBackendBitmask)
{
	return GetBackendPref("gfx.content.azure.backends", aBackendBitmask);
}

/* static */ BackendType
gfxPlatform::GetBackendPref(const char* aBackendPrefName, uint32_t &aBackendBitmask)
{
	nsTArray<nsCString> backendList;
	nsCString prefString;
	if (NS_SUCCEEDED(Preferences::GetCString(aBackendPrefName, &prefString))) {
		ParseString(prefString, ',', backendList);
	}

	uint32_t allowedBackends = 0;
	BackendType result = BackendType::NONE;
	for (uint32_t i = 0; i < backendList.Length(); ++i) {
		BackendType type = BackendTypeForName(backendList[i]);
		if (BackendTypeBit(type) & aBackendBitmask) {
			allowedBackends |= BackendTypeBit(type);
			if (result == BackendType::NONE) {
				result = type;
			}
		}
	}

	aBackendBitmask = allowedBackends;
	return result;
}

bool
gfxPlatform::InSafeMode()
{
	static bool sSafeModeInitialized = false;
	static bool sInSafeMode = false;

	if (!sSafeModeInitialized) {
		sSafeModeInitialized = true;
		nsCOMPtr<nsIXULRuntime> xr = do_GetService("@mozilla.org/xre/runtime;1");
		if (xr) {
			xr->GetInSafeMode(&sInSafeMode);
		}
	}
	return sInSafeMode;
}

eCMSMode
gfxPlatform::GetCMSMode()
{
	if (!gCMSInitialized) {
		int32_t mode = gfxPrefs::CMSMode();
		if (mode >= 0 && mode < eCMSMode_AllCount) {
			gCMSMode = static_cast<eCMSMode>(mode);
		}

		bool enableV4 = gfxPrefs::CMSEnableV4();
		if (enableV4) {
			qcms_enable_iccv4();
		}
		gCMSInitialized = true;
	}
	return gCMSMode;
}

int
gfxPlatform::GetRenderingIntent()
{
	// gfxPrefs.h is using 0 as the default for the rendering
	// intent preference, based on that being the value for
	// QCMS_INTENT_DEFAULT.  Assert here to catch if that ever
	// changes and we can then figure out what to do about it.
	MOZ_ASSERT(QCMS_INTENT_DEFAULT == 0);

	/* Try to query the pref system for a rendering intent. */
	int32_t pIntent = gfxPrefs::CMSRenderingIntent();
	if ((pIntent < QCMS_INTENT_MIN) || (pIntent > QCMS_INTENT_MAX)) {
		/* If the pref is out of range, use embedded profile. */
		pIntent = -1;
	}
	return pIntent;
}

void
gfxPlatform::TransformPixel(const Color& in, Color& out, qcms_transform *transform)
{

	if (transform) {
		/* we want the bytes in RGB order */
#ifdef IS_LITTLE_ENDIAN
		/* ABGR puts the bytes in |RGBA| order on little endian */
		uint32_t packed = in.ToABGR();
		qcms_transform_data(transform,
				(uint8_t *)&packed, (uint8_t *)&packed,
				1);
		out = Color::FromABGR(packed);
#else
	/* ARGB puts the bytes in |ARGB| order on big endian */
		uint32_t packed = in.UnusualToARGB();
		/* add one to move past the alpha byte */
		qcms_transform_data(transform,
				(uint8_t *)&packed + 1, (uint8_t *)&packed + 1,
				1);
		out = Color::UnusualFromARGB(packed);
#endif
	}

	else if (&out != &in)
		out = in;
}

void
gfxPlatform::GetPlatformCMSOutputProfile(void *&mem, size_t &size)
{
	mem = nullptr;
	size = 0;
}

void
gfxPlatform::GetCMSOutputProfileData(void *&mem, size_t &size)
{
	nsAdoptingCString fname = Preferences::GetCString("gfx.color_management.display_profile");
	if (!fname.IsEmpty()) {
		qcms_data_from_path(fname, &mem, &size);
	}
	else {
		gfxPlatform::GetPlatform()->GetPlatformCMSOutputProfile(mem, size);
	}
}

void
gfxPlatform::CreateCMSOutputProfile()
{
	if (!gCMSOutputProfile) {
		/* Determine if we're using the internal override to force sRGB as
           an output profile for reftests. See Bug 452125.

           Note that we don't normally (outside of tests) set a
           default value of this preference, which means nsIPrefBranch::GetBoolPref
           will typically throw (and leave its out-param untouched).
		 */
		if (Preferences::GetBool(GFX_PREF_CMS_FORCE_SRGB, false)) {
			gCMSOutputProfile = GetCMSsRGBProfile();
		}

		if (!gCMSOutputProfile) {
			void* mem = nullptr;
			size_t size = 0;

			GetCMSOutputProfileData(mem, size);
			if ((mem != nullptr) && (size > 0)) {
				gCMSOutputProfile = qcms_profile_from_memory(mem, size);
				free(mem);
			}
		}

		/* Determine if the profile looks bogus. If so, close the profile
		 * and use sRGB instead. See bug 460629, */
		if (gCMSOutputProfile && qcms_profile_is_bogus(gCMSOutputProfile)) {
			NS_ASSERTION(gCMSOutputProfile != GetCMSsRGBProfile(),
					"Builtin sRGB profile tagged as bogus!!!");
			qcms_profile_release(gCMSOutputProfile);
			gCMSOutputProfile = nullptr;
		}

		if (!gCMSOutputProfile) {
			gCMSOutputProfile = GetCMSsRGBProfile();
		}
		/* Precache the LUT16 Interpolations for the output profile. See
           bug 444661 for details. */
		qcms_profile_precache_output_transform(gCMSOutputProfile);
	}
}

qcms_profile *
gfxPlatform::GetCMSOutputProfile()
{
	return gCMSOutputProfile;
}

qcms_profile *
gfxPlatform::GetCMSsRGBProfile()
{
	if (!gCMSsRGBProfile) {

		/* Create the profile using qcms. */
		gCMSsRGBProfile = qcms_profile_sRGB();
	}
	return gCMSsRGBProfile;
}

qcms_transform *
gfxPlatform::GetCMSRGBTransform()
{
	if (!gCMSRGBTransform) {
		qcms_profile *inProfile, *outProfile;
		outProfile = GetCMSOutputProfile();
		inProfile = GetCMSsRGBProfile();

		if (!inProfile || !outProfile)
			return nullptr;

		gCMSRGBTransform = qcms_transform_create(inProfile, QCMS_DATA_RGB_8,
				outProfile, QCMS_DATA_RGB_8,
				QCMS_INTENT_PERCEPTUAL);
	}

	return gCMSRGBTransform;
}

qcms_transform *
gfxPlatform::GetCMSInverseRGBTransform()
{
	if (!gCMSInverseRGBTransform) {
		qcms_profile *inProfile, *outProfile;
		inProfile = GetCMSOutputProfile();
		outProfile = GetCMSsRGBProfile();

		if (!inProfile || !outProfile)
			return nullptr;

		gCMSInverseRGBTransform = qcms_transform_create(inProfile, QCMS_DATA_RGB_8,
				outProfile, QCMS_DATA_RGB_8,
				QCMS_INTENT_PERCEPTUAL);
	}

	return gCMSInverseRGBTransform;
}

qcms_transform *
gfxPlatform::GetCMSRGBATransform()
{
	if (!gCMSRGBATransform) {
		qcms_profile *inProfile, *outProfile;
		outProfile = GetCMSOutputProfile();
		inProfile = GetCMSsRGBProfile();

		if (!inProfile || !outProfile)
			return nullptr;

		gCMSRGBATransform = qcms_transform_create(inProfile, QCMS_DATA_RGBA_8,
				outProfile, QCMS_DATA_RGBA_8,
				QCMS_INTENT_PERCEPTUAL);
	}

	return gCMSRGBATransform;
}

/* Shuts down various transforms and profiles for CMS. */
static void ShutdownCMS()
{

	if (gCMSRGBTransform) {
		qcms_transform_release(gCMSRGBTransform);
		gCMSRGBTransform = nullptr;
	}
	if (gCMSInverseRGBTransform) {
		qcms_transform_release(gCMSInverseRGBTransform);
		gCMSInverseRGBTransform = nullptr;
	}
	if (gCMSRGBATransform) {
		qcms_transform_release(gCMSRGBATransform);
		gCMSRGBATransform = nullptr;
	}
	if (gCMSOutputProfile) {
		qcms_profile_release(gCMSOutputProfile);

		// handle the aliased case
		if (gCMSsRGBProfile == gCMSOutputProfile)
			gCMSsRGBProfile = nullptr;
		gCMSOutputProfile = nullptr;
	}
	if (gCMSsRGBProfile) {
		qcms_profile_release(gCMSsRGBProfile);
		gCMSsRGBProfile = nullptr;
	}

	// Reset the state variables
	gCMSMode = eCMSMode_Off;
	gCMSInitialized = false;
}

// default SetupClusterBoundaries, based on Unicode properties;
// platform subclasses may override if they wish
void
gfxPlatform::SetupClusterBoundaries(gfxTextRun *aTextRun, const char16_t *aString)
{
	if (aTextRun->GetFlags() & gfxTextRunFactory::TEXT_IS_8BIT) {
		// 8-bit text doesn't have clusters.
		// XXX is this true in all languages???
		// behdad: don't think so.  Czech for example IIRC has a
		// 'ch' grapheme.
		// jfkthame: but that's not expected to behave as a grapheme cluster
		// for selection/editing/etc.
		return;
	}

	aTextRun->SetupClusterBoundaries(0, aString, aTextRun->GetLength());
}

int32_t
gfxPlatform::GetBidiNumeralOption()
{
	if (mBidiNumeralOption == UNINITIALIZED_VALUE) {
		mBidiNumeralOption = Preferences::GetInt(BIDI_NUMERAL_PREF, 0);
	}
	return mBidiNumeralOption;
}

/* static */ void
gfxPlatform::FlushFontAndWordCaches()
{
	gfxFontCache *fontCache = gfxFontCache::GetCache();
	if (fontCache) {
		fontCache->AgeAllGenerations();
		fontCache->FlushShapedWordCaches();
	}

	gfxPlatform::PurgeSkiaFontCache();
}

void
gfxPlatform::FontsPrefsChanged(const char *aPref)
{
	NS_ASSERTION(aPref != nullptr, "null preference");
	if (!strcmp(GFX_DOWNLOADABLE_FONTS_ENABLED, aPref)) {
		mAllowDownloadableFonts = UNINITIALIZED_VALUE;
	} else if (!strcmp(GFX_PREF_FALLBACK_USE_CMAPS, aPref)) {
		mFallbackUsesCmaps = UNINITIALIZED_VALUE;
	} else if (!strcmp(GFX_PREF_WORD_CACHE_CHARLIMIT, aPref)) {
		mWordCacheCharLimit = UNINITIALIZED_VALUE;
		FlushFontAndWordCaches();
	} else if (!strcmp(GFX_PREF_WORD_CACHE_MAXENTRIES, aPref)) {
		mWordCacheMaxEntries = UNINITIALIZED_VALUE;
		FlushFontAndWordCaches();
	} else if (!strcmp(GFX_PREF_GRAPHITE_SHAPING, aPref)) {
		mGraphiteShapingEnabled = UNINITIALIZED_VALUE;
		FlushFontAndWordCaches();
	} else if (!strcmp(BIDI_NUMERAL_PREF, aPref)) {
		mBidiNumeralOption = UNINITIALIZED_VALUE;
	} else if (!strcmp(GFX_PREF_OPENTYPE_SVG, aPref)) {
		mOpenTypeSVGEnabled = UNINITIALIZED_VALUE;
		gfxFontCache::GetCache()->AgeAllGenerations();
	}
}


mozilla::LogModule*
gfxPlatform::GetLog(eGfxLog aWhichLog)
{

	return nullptr;
}

mozilla::gfx::SurfaceFormat
gfxPlatform::Optimal2DFormatForContent(gfxContentType aContent)
{
	switch (aContent) {
	case gfxContentType::COLOR:
		switch (GetOffscreenFormat()) {
		case SurfaceFormat::A8R8G8B8_UINT32:
			return mozilla::gfx::SurfaceFormat::B8G8R8A8;
		case SurfaceFormat::X8R8G8B8_UINT32:
			return mozilla::gfx::SurfaceFormat::B8G8R8X8;
		case SurfaceFormat::R5G6B5_UINT16:
			return mozilla::gfx::SurfaceFormat::R5G6B5_UINT16;
		default:
			NS_NOTREACHED("unknown gfxImageFormat for gfxContentType::COLOR");
			return mozilla::gfx::SurfaceFormat::B8G8R8A8;
		}
		case gfxContentType::ALPHA:
			return mozilla::gfx::SurfaceFormat::A8;
		case gfxContentType::COLOR_ALPHA:
			return mozilla::gfx::SurfaceFormat::B8G8R8A8;
		default:
			NS_NOTREACHED("unknown gfxContentType");
			return mozilla::gfx::SurfaceFormat::B8G8R8A8;
	}
}

gfxImageFormat
gfxPlatform::OptimalFormatForContent(gfxContentType aContent)
{
	switch (aContent) {
	case gfxContentType::COLOR:
		return GetOffscreenFormat();
	case gfxContentType::ALPHA:
		return SurfaceFormat::A8;
	case gfxContentType::COLOR_ALPHA:
		return SurfaceFormat::A8R8G8B8_UINT32;
	default:
		NS_NOTREACHED("unknown gfxContentType");
		return SurfaceFormat::A8R8G8B8_UINT32;
	}
}


already_AddRefed<ScaledFont>
gfxPlatform::GetScaledFontForFontWithCairoSkia(DrawTarget* aTarget, gfxFont* aFont)
{
	NativeFont nativeFont;
	if (aTarget->GetBackendType() == BackendType::CAIRO || aTarget->GetBackendType() == BackendType::SKIA) {
		nativeFont.mType = NativeFontType::CAIRO_FONT_FACE;
		nativeFont.mFont = aFont->GetCairoScaledFont();
		return Factory::CreateScaledFontForNativeFont(nativeFont, aFont->GetAdjustedSize());
	}

	return nullptr;
}

/* static */ bool
gfxPlatform::IsInLayoutAsapMode()
{
	// There are 2 modes of ASAP mode.
	// 1 is that the refresh driver and compositor are in lock step
	// the second is that the compositor goes ASAP and the refresh driver
	// goes at whatever the configurated rate is. This only checks the version
	// talos uses, which is the refresh driver and compositor are in lockstep.
	return gfxPrefs::LayoutFrameRate() == 0;
}

/*static*/ bool
gfxPlatform::AsyncPanZoomEnabled()
{
#if !defined(MOZ_B2G) && !defined(MOZ_WIDGET_ANDROID) && !defined(MOZ_WIDGET_UIKIT)
	// For XUL applications (everything but B2G on mobile and desktop, and
	// Firefox on Android) we only want to use APZ when E10S is enabled. If
	// we ever get input events off the main thread we can consider relaxing
	// this requirement.
	if (!BrowserTabsRemoteAutostart()) {
		return false;
	}
#endif
#ifdef MOZ_ANDROID_APZ
	return true;
#else
	return gfxPrefs::AsyncPanZoomEnabledDoNotUseDirectly();
#endif
}

/*virtual*/ bool
gfxPlatform::UseProgressivePaint()
{
	return gfxPrefs::ProgressivePaintDoNotUseDirectly();
}

/*static*/ bool
gfxPlatform::PerfWarnings()
{
	return gfxPrefs::PerfWarnings();
}

bool
gfxPlatform::SupportsApzDragInput() const
{
	return gfxPrefs::APZDragEnabled();
}

void
gfxPlatform::BumpDeviceCounter()
{
	mDeviceCounter++;
}
