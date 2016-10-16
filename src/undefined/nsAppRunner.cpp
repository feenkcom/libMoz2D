/*
 * nsAppRunner.cpp
 *
 *  Created on: Aug 1, 2016
 *      Author: Aliaksei Syrel
 */

#include "nsAppRunner.h"

namespace mozilla {

bool BrowserTabsRemoteAutostart() {
	return false;
}

} // namespace mozilla

bool
XRE_IsParentProcess()
{
  return true;
}

bool
XRE_IsContentProcess()
{
  return false;
}

GeckoProcessType  XRE_GetProcessType  () {
	return GeckoProcessType_Default;
}
