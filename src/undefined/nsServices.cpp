/*
 * nsServices.cpp
 *
 *  Created on: Aug 1, 2016
 */

#include "stub.h"

#include "mozilla/Services.h"
#include "nsThreadManager.h"
#include "nsThreadUtils.h"


nsIThread*
NS_GetCurrentThread()
{
	STUB("NS_GetCurrentThread");
}

EXPORT_XPCOM_API(void)
NS_DebugBreak(uint32_t aSeverity, const char* aStr, const char* aExpr, const char* aFile, int32_t aLine)
{}

void
NS_SetThreadName(nsIThread* aThread, const nsACString& aName)
{

}
