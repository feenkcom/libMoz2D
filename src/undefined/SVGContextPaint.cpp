/*
 * SVGContextPaint.cpp
 *
 *  Created on: Aug 6, 2016
 *      Author: Aliaksei Syrel
 */


#include "SVGContextPaint.h"

#include "gfxContext.h"
#include "mozilla/gfx/2D.h"
#include "nsIDocument.h"
#include "nsSVGPaintServerFrame.h"
#include "nsSVGEffects.h"
#include "nsSVGPaintServerFrame.h"

using namespace mozilla::gfx;

namespace mozilla {

void
SVGContextPaint::InitStrokeGeometry(gfxContext* aContext,
                                    float devUnitsPerSVGUnit)
{
  mStrokeWidth = aContext->CurrentLineWidth() / devUnitsPerSVGUnit;
  aContext->CurrentDash(mDashes, &mDashOffset);
  for (uint32_t i = 0; i < mDashes.Length(); i++) {
    mDashes[i] /= devUnitsPerSVGUnit;
  }
  mDashOffset /= devUnitsPerSVGUnit;
}

}
