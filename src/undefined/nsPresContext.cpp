/*
 * nsPresContext.cpp
 *
 *  Created on: Aug 1, 2016
 *      Author: Aliaksei Syrel
 */

#include "nsPresContext.h"
#include "mozilla/Preferences.h"

int32_t
nsPresContext::AppUnitsPerDevPixel() const
{
  return mozilla::Preferences::GetInt("gfx.app_units_per_dev_pixel");
}
