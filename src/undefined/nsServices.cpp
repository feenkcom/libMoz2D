/*
 * nsServices.cpp
 *
 *  Created on: Aug 1, 2016
 */

#include "stub.h"

#include "mozilla/Services.h"
#include "nsThreadManager.h"
#include "nsThreadUtils.h"

already_AddRefed<nsIXULChromeRegistry> mozilla::services::GetXULChromeRegistryService() {
	STUB("GetXULChromeRegistryService");
}

already_AddRefed<nsIToolkitChromeRegistry> mozilla::services::GetToolkitChromeRegistryService() {
	STUB("GetXULChromeRegistryService");
}

already_AddRefed<nsIObserverService> mozilla::services::GetObserverService() {
	STUB("GetObserverService");
}

already_AddRefed<nsIGfxInfo> mozilla::services::GetGfxInfo(){
	return nullptr;
}

nsIThread*
NS_GetCurrentThread()
{
	STUB("NS_GetCurrentThread");
}

EXPORT_XPCOM_API(void)
NS_DebugBreak(uint32_t aSeverity, const char* aStr, const char* aExpr, const char* aFile, int32_t aLine)
{}

nsresult
NS_GetComponentManager(nsIComponentManager** aResult)
{
  return NS_ERROR_NOT_INITIALIZED;
}

nsresult
NS_GetServiceManager(nsIServiceManager** aResult)
{
	  return NS_ERROR_NOT_INITIALIZED;
}

void
NS_SetThreadName(nsIThread* aThread, const nsACString& aName)
{

}
