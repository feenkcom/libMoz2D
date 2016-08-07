/*
 * nsWidget.cpp
 *
 *  Created on: Aug 6, 2016
 *      Author: Aliaksei Syrel
 */


#include "nsBaseWidget.h"
#include "mozilla/Preferences.h"
#include "prdtoa.h"

using namespace mozilla;

float nsBaseWidget::GetDPI()
{
  return 96.0f;
}

CSSToLayoutDeviceScale nsIWidget::GetDefaultScale()
{
  double devPixelsPerCSSPixel = DefaultScaleOverride();

  if (devPixelsPerCSSPixel <= 0.0) {
    devPixelsPerCSSPixel = GetDefaultScaleInternal();
  }

  return CSSToLayoutDeviceScale(devPixelsPerCSSPixel);
}

/* static */
double nsIWidget::DefaultScaleOverride()
{
  // The number of device pixels per CSS pixel. A value <= 0 means choose
  // automatically based on the DPI. A positive value is used as-is. This effectively
  // controls the size of a CSS "px".
  double devPixelsPerCSSPixel = -1.0;

  nsAdoptingCString prefString = Preferences::GetCString("layout.css.devPixelsPerPx");
  if (!prefString.IsEmpty()) {
    devPixelsPerCSSPixel = PR_strtod(prefString, nullptr);
  }

  return devPixelsPerCSSPixel;
}
