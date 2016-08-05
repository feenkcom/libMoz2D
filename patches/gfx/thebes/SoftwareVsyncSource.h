/* -*- Mode: C++; tab-width: 20; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef GFX_SOFTWARE_VSYNC_SOURCE_H
#define GFX_SOFTWARE_VSYNC_SOURCE_H

#include "mozilla/Monitor.h"
#include "mozilla/RefPtr.h"
#include "mozilla/TimeStamp.h"
#include "base/thread.h"
#include "nsISupportsImpl.h"
#include "VsyncSource.h"

// Fallback option to use a software timer to mimic vsync. Useful for gtests
// To mimic a hardware vsync thread, we create a dedicated software timer
// vsync thread.
class SoftwareVsyncSource : public mozilla::gfx::VsyncSource
{
public:
  SoftwareVsyncSource();
  ~SoftwareVsyncSource();

};

#endif /* GFX_SOFTWARE_VSYNC_SOURCE_H */
