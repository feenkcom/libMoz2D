//
// Created by Aliaksei Syrel on 10.11.17.
//

#include "font_user.h"
#include "nsString.h"

#include "debug.h"

// We don't support downloading fonts by URI, so we can just implement stub methods (they will not be called anyway)
nsresult UserFontSet::CheckFontLoad(const gfxFontFaceSrc *aFontFaceSrc, nsIPrincipal **aPrincipal, bool *aBypassCache) { return NS_OK; }
bool UserFontSet::IsFontLoadAllowed(nsIURI *aFontLocation, nsIPrincipal *aPrincipal) { return false; }
nsresult UserFontSet::StartLoad(gfxUserFontEntry *aUserFontEntry, const gfxFontFaceSrc *aFontFaceSrc) { return NS_OK; }
void UserFontSet::RecordFontLoadDone(uint32_t aFontSize, mozilla::TimeStamp aDoneTime) {}
bool UserFontSet::GetPrivateBrowsing() { return false; }
nsresult UserFontSet::SyncLoadFontData(gfxUserFontEntry *aFontToLoad, const gfxFontFaceSrc *aFontFaceSrc, uint8_t *&aBuffer, uint32_t &aBufferLength) { return NS_OK; }
void UserFontSet::DoRebuildUserFontSet() {}
nsresult UserFontSet::LogMessage(gfxUserFontEntry *aUserFontEntry, const char *aMessage, uint32_t aFlags, nsresult aStatus) {
    moz2d_print_stderr(aMessage);
    return NS_OK;
}

already_AddRefed<gfxUserFontEntry> UserFontSet::CreateUserFontEntry(
        const nsTArray<gfxFontFaceSrc>& aFontFaceSrcList,
        uint32_t aWeight,
        int32_t aStretch,
        uint8_t aStyle,
        const nsTArray<gfxFontFeature>& aFeatureSettings,
        uint32_t aLanguageOverride,
        gfxSparseBitSet* aUnicodeRanges,
        uint8_t aFontDisplay)
{

    RefPtr<gfxUserFontEntry> userFontEntry =
            new gfxUserFontEntry(this, aFontFaceSrcList, aWeight,
                                 aStretch, aStyle, aFeatureSettings,
                                 aLanguageOverride, aUnicodeRanges, aFontDisplay);
    return userFontEntry.forget();
}

void
FontFaceBufferSource::TakeBuffer(uint8_t*& aBuffer, uint32_t& aLength)
{
    aBuffer = (uint8_t*) malloc(sizeof(uint8_t) * mSourceBufferLength);
    if (aBuffer == NULL) {
        return;
    }
    memcpy((void*)aBuffer, (void*)mSourceBuffer , sizeof(uint8_t) * mSourceBufferLength);

    aLength = mSourceBufferLength;
}

UserFontSet* moz2d_font_user_set_create () {
    UserFontSet* aUserFontSet = new UserFontSet();
    // Need to increase reference, because my caller gets ownership over font set
    aUserFontSet->AddRef();
    return aUserFontSet;
}

// Caller owns a newly created font entry

gfxUserFontEntry* moz2d_font_user_find_or_create_user_font_entry (
        UserFontSet* aUserFontSet,
        const char* aFamilyName,
        uint8_t* aBuffer,
        uint32_t aBufferLength,
        gfxFontStyle* aFontStyle) {

    nsTArray<gfxFontFaceSrc> srcArray;
    nsTArray<gfxFontFeature> featureSettings;
    nsAutoPtr<gfxCharacterMap> unicodeRanges;

    gfxFontFaceSrc* faceSrc = srcArray.AppendElement();
    if (!faceSrc)
        return nullptr;



    RefPtr<FontFaceBufferSource> bufferSource = new FontFaceBufferSource(aBuffer, aBufferLength);

    faceSrc->mSourceType = gfxFontFaceSrc::eSourceType_Buffer;
    faceSrc->mBuffer = bufferSource;

    unicodeRanges = new gfxCharacterMap();

    return aUserFontSet->FindOrCreateUserFontEntry(
            NS_ConvertUTF8toUTF16(aFamilyName),
            srcArray,
            aFontStyle->weight,
            aFontStyle->stretch,
            aFontStyle->style,
            featureSettings,
            NO_FONT_LANGUAGE_OVERRIDE,
            unicodeRanges,
            NS_FONT_DISPLAY_AUTO).take();

}

void moz2d_font_user_entry_add(
        UserFontSet* aUserFontSet,
        const char* aFamilyName, // UTF-8
        gfxUserFontEntry* aUserFontEntry) {
    aUserFontSet->AddUserFontEntry(NS_ConvertUTF8toUTF16(aFamilyName), aUserFontEntry);
}


void moz2d_font_user_entry_load(gfxUserFontEntry* aFontEntry) {
    aFontEntry->Load();
}

gfxUserFontEntry::UserFontLoadState moz2d_font_user_entry_get_load_state(gfxUserFontEntry* aFontEntry) {
    return aFontEntry->LoadState();
}