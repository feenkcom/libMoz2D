/*
 * SVGContextPaint.cpp
 *
 *  Created on: Aug 6, 2016
 *      Author: Aliaksei Syrel
 */


#include "nsSVGUtils.h"

using namespace mozilla;
using namespace mozilla::dom;
using namespace mozilla::gfx;

bool
nsSVGUtils::PaintSVGGlyph(Element* aElement, gfxContext* aContext,
                          gfxTextContextPaint* aContextPaint)
{
  return false;
}

bool
nsSVGUtils::GetSVGGlyphExtents(Element* aElement,
                               const gfxMatrix& aSVGToAppSpace,
                               gfxRect* aResult)
{
  return false;
}

