/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "gfxSVGGlyphs.h"

#include "nsIDocument.h"
#include "nsIContentViewer.h"
#include "nsIPresShell.h"

#include <string.h>
#include <stdexcept>
#include "nsString.h"

#define STUB(X) throw std::runtime_error (ToNewUTF8String(NS_LITERAL_STRING("[") + NS_LITERAL_STRING(X) + NS_LITERAL_STRING("] Stub method! Must not be called during runtime.")));

using namespace mozilla::gfx;

/* static */ const Color SimpleTextContextPaint::sZero = Color();

void gfxTextContextPaint::InitStrokeGeometry(gfxContext *aContext, float devUnitsPerSVGUnit) {
	STUB("gfxTextContextPaint::InitStrokeGeometry");
}

gfxSVGGlyphs::gfxSVGGlyphs(hb_blob_t *aSVGTable, gfxFontEntry *aFontEntry) {
	STUB("gfxSVGGlyphs::gfxSVGGlyphs");
}

gfxSVGGlyphs::~gfxSVGGlyphs() {
	STUB("gfxSVGGlyphs::~gfxSVGGlyphs");
}

bool gfxSVGGlyphs::HasSVGGlyph(uint32_t aGlyphId) {
	STUB("gfxSVGGlyphs::HasSVGGlyph");
	return false;
}

bool gfxSVGGlyphs::RenderGlyph(gfxContext *aContext, uint32_t aGlyphId, gfxTextContextPaint *aContextPaint) {
	STUB("gfxSVGGlyphs::RenderGlyph");
	return false;
}

bool gfxSVGGlyphs::GetGlyphExtents(uint32_t aGlyphId, const gfxMatrix& aSVGToAppSpace, gfxRect *aResult) {
	STUB("gfxSVGGlyphs::GetGlyphExtents");
	return false;
}

gfxSVGGlyphsDocument::gfxSVGGlyphsDocument(const uint8_t *aBuffer,
                                           uint32_t aBufLen,
                                           gfxSVGGlyphs *aSVGGlyphs)
    : mOwner(aSVGGlyphs)
{
	STUB("gfxSVGGlyphsDocument::~gfxSVGGlyphsDocument");
}

gfxSVGGlyphsDocument::~gfxSVGGlyphsDocument() {
	STUB("gfxSVGGlyphsDocument::~gfxSVGGlyphsDocument");
}

void gfxSVGGlyphsDocument::DidRefresh() {
	STUB("gfxSVGGlyphsDocument::DidRefresh");
}

size_t
gfxSVGGlyphs::SizeOfIncludingThis(mozilla::MallocSizeOf aMallocSizeOf) const
{
    // We don't include the size of mSVGData here, because (depending on the
    // font backend implementation) it will either wrap a block of data owned
    // by the system (and potentially shared), or a table that's in our font
    // table cache and therefore already counted.
    size_t result = aMallocSizeOf(this);
    return result;
}
