/*
 * nsServices.cpp
 *
 *  Created on: Aug 1, 2016
 */

#include "stub.h"

#include "mozilla/Services.h"

already_AddRefed<nsIXULChromeRegistry> mozilla::services::GetXULChromeRegistryService() {
	STUB("GetXULChromeRegistryService");
}

already_AddRefed<nsIToolkitChromeRegistry> mozilla::services::GetToolkitChromeRegistryService() {
	STUB("GetXULChromeRegistryService");
}

already_AddRefed<nsIObserverService> mozilla::services::GetObserverService() {
	STUB("GetObserverService");
}
