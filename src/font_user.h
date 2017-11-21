//
// Created by Aliaksei Syrel on 10.11.17.
//

#ifndef MOZ2D_FONT_USER_H
#define MOZ2D_FONT_USER_H

#include "exports.h"
#include "2d/2D.h"
#include "gfxUserFontSet.h"

using namespace mozilla;
using namespace mozilla::gfx;


class UserFontSet final : public gfxUserFontSet
{
public:
    explicit UserFontSet() {}

    virtual nsresult CheckFontLoad(const gfxFontFaceSrc* aFontFaceSrc,
                                   nsIPrincipal** aPrincipal,
                                   bool* aBypassCache) override;

    virtual bool IsFontLoadAllowed(nsIURI* aFontLocation,
                                   nsIPrincipal* aPrincipal) override;

    virtual nsresult StartLoad(gfxUserFontEntry* aUserFontEntry,
                               const gfxFontFaceSrc* aFontFaceSrc) override;

    void RecordFontLoadDone(uint32_t aFontSize,
                            mozilla::TimeStamp aDoneTime) override;

protected:
    virtual bool GetPrivateBrowsing() override;
    virtual nsresult SyncLoadFontData(gfxUserFontEntry* aFontToLoad,
                                      const gfxFontFaceSrc* aFontFaceSrc,
                                      uint8_t*& aBuffer,
                                      uint32_t& aBufferLength) override;
    virtual nsresult LogMessage(gfxUserFontEntry* aUserFontEntry,
                                const char* aMessage,
                                uint32_t aFlags = nsIScriptError::errorFlag,
                                nsresult aStatus = NS_OK) override;
    virtual void DoRebuildUserFontSet() override;
    virtual already_AddRefed<gfxUserFontEntry> CreateUserFontEntry(
            const nsTArray<gfxFontFaceSrc>& aFontFaceSrcList,
            uint32_t aWeight,
            int32_t aStretch,
            uint8_t aStyle,
            const nsTArray<gfxFontFeature>& aFeatureSettings,
            uint32_t aLanguageOverride,
            gfxSparseBitSet* aUnicodeRanges,
            uint8_t aFontDisplay) override;
};

/**
 * An object that wraps a FontFace object and exposes its ArrayBuffer
 * or ArrayBufferView data in a form the user font set can consume.
 */
class FontFaceBufferSource : public gfxFontFaceBufferSource
{
public:
    explicit FontFaceBufferSource(uint8_t* aSourceBuffer, uint32_t aSourceBufferLength)
            : mSourceBuffer(aSourceBuffer), mSourceBufferLength(aSourceBufferLength) {}
    virtual void TakeBuffer(uint8_t*& aBuffer, uint32_t& aLength);
    virtual bool Equals(gfxFontFaceBufferSource* other);

public:
    uint8_t* mSourceBuffer = nullptr;
    uint32_t mSourceBufferLength = 0;
};

#ifdef __cplusplus
extern "C" {
#endif

LIBRARY_API UserFontSet* moz2d_font_user_set_create ();
LIBRARY_API void moz2d_font_user_set_release (UserFontSet* aUserFontSet);

LIBRARY_API gfxUserFontEntry* moz2d_font_user_find_or_create_user_font_entry(
        UserFontSet* aUserFontSet,
        const char* aFamilyName, // UTF-8
        uint8_t* aBuffer,
        uint32_t aLength,
        gfxFontStyle* aFontStyle // only uses weight, stretch and style
);



LIBRARY_API void moz2d_font_user_entry_add (
        UserFontSet* aUserFontSet,
        const char* aFamilyName, // UTF-8
        gfxUserFontEntry* aUserFontEntry);

LIBRARY_API void moz2d_font_user_entry_release(gfxUserFontEntry* aUserFontEntry);
		
LIBRARY_API bool moz2d_font_user_set_has_family(UserFontSet* aUserFontSet, const char* aFamilyName); // UTF-8

LIBRARY_API void moz2d_font_user_entry_load(gfxUserFontEntry* aFontEntry);
LIBRARY_API gfxUserFontEntry::UserFontLoadState moz2d_font_user_entry_get_load_state(gfxUserFontEntry* aFontEntry);

#ifdef __cplusplus
}
#endif


#endif //MOZ2D_FONT_USER_H
