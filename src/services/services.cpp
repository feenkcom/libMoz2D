/*
 * services.cpp
 *
 *  Created on: Jul 31, 2016
 *      Author: Aliaksei Syrel
 */

#include "services.h"

#include "nsGkAtoms.h"
#include "nsAtomService.h"
#include "nsAtomTable.h"
#include "gfxPlatform.h"
#include "gfxConfig.h"
#include "nsISupportsUtils.h"
#include "ModuleUtils.h"

#include "nsXPCOMCIDInternal.h"
#include "nsXPCOMPrivate.h"
/*
 * Services from XPCOMModule.inc
 */
#include "nsCycleCollector.h"
#include "nsObserverList.h"
#include "nsObserverService.h"
#include "nsProperties.h"
#include "nsPersistentProperties.h"
#include "nsScriptableInputStream.h"
#include "nsBinaryStream.h"
#include "nsStorageStream.h"
#include "nsPipe.h"
#include "nsScriptableBase64Encoder.h"

#include "nsMemoryImpl.h"
#include "nsDebugImpl.h"
#include "nsTraceRefcnt.h"
#include "nsErrorService.h"

#include "nsSupportsArray.h"
#include "nsArray.h"
#include "nsINIParserImpl.h"
#include "nsSupportsPrimitives.h"

#include "nsComponentManager.h"
#include "nsCategoryManagerUtils.h"
#include "nsIServiceManager.h"

#include "nsThreadManager.h"
#include "nsThreadPool.h"

#include "xptinfo.h"
#include "nsIInterfaceInfoManager.h"
#include "xptiprivate.h"
#include "mozilla/XPTInterfaceInfoManager.h"

#include "nsTimerImpl.h"
#include "TimerThread.h"

#include "nsThread.h"
#include "nsProcess.h"
#include "nsEnvironment.h"
#include "nsVersionComparatorImpl.h"

#include "nsIFile.h"
#include "nsLocalFile.h"
#if defined(XP_UNIX)
#include "nsNativeCharsetUtils.h"
#endif
#include "nsDirectoryService.h"
#include "nsDirectoryServiceDefs.h"
#include "nsCategoryManager.h"
#include "nsICategoryManager.h"
#include "nsMultiplexInputStream.h"

#include "nsStringStream.h"
extern nsresult nsStringInputStreamConstructor(nsISupports*, REFNSIID, void**);

#include "nsAtomService.h"
#include "nsAtomTable.h"
#include "nsISupportsImpl.h"

#include "nsHashPropertyBag.h"

#include "nsUnicharInputStream.h"
#include "nsVariant.h"

#include "nsUUIDGenerator.h"

#include "nsIOUtil.h"

#include "SpecialSystemDirectory.h"

#if defined(XP_WIN)
#include "mozilla/WindowsVersion.h"
#include "nsWindowsRegKey.h"
#endif

#ifdef MOZ_WIDGET_COCOA
#include "nsMacUtilsImpl.h"
#endif

#include "nsSystemInfo.h"
#include "nsMemoryReporterManager.h"
#include "nsMemoryInfoDumper.h"
#include "nsSecurityConsoleMessage.h"
#include "nsMessageLoop.h"

#include "nsStatusReporterManager.h"

#include "nsChromeRegistry.h"
#include "nsChromeProtocolHandler.h"
#include "mozilla/PoisonIOInterposer.h"
#include "mozilla/LateWriteChecks.h"

#include "mozilla/Services.h"
#include "mozilla/Preferences.h"

#include <iostream>

using namespace mozilla;
using namespace std;

// Registry Factory creation function defined in nsRegistry.cpp
// We hook into this function locally to create and register the registry
// Since noone outside xpcom needs to know about this and nsRegistry.cpp
// does not have a local include file, we are putting this definition
// here rather than in nsIRegistry.h
extern nsresult NS_RegistryGetFactory(nsIFactory** aFactory);
extern nsresult NS_CategoryManagerGetFactory(nsIFactory**);

#ifdef XP_WIN
extern nsresult CreateAnonTempFileRemover();
#endif

NS_GENERIC_FACTORY_CONSTRUCTOR(nsProcess)

NS_GENERIC_FACTORY_CONSTRUCTOR(nsSupportsID)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsSupportsString)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsSupportsCString)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsSupportsPRBool)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsSupportsPRUint8)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsSupportsPRUint16)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsSupportsPRUint32)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsSupportsPRUint64)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsSupportsPRTime)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsSupportsChar)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsSupportsPRInt16)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsSupportsPRInt32)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsSupportsPRInt64)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsSupportsFloat)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsSupportsDouble)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsSupportsVoid)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsSupportsInterfacePointer)

NS_GENERIC_FACTORY_CONSTRUCTOR(nsAtomService)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsTimerImpl)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsBinaryOutputStream)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsBinaryInputStream)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsStorageStream)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsVersionComparatorImpl)
NS_GENERIC_FACTORY_CONSTRUCTOR(nsScriptableBase64Encoder)

NS_GENERIC_FACTORY_CONSTRUCTOR(nsVariantCC)

NS_GENERIC_FACTORY_CONSTRUCTOR(nsHashPropertyBagCC)

NS_GENERIC_AGGREGATED_CONSTRUCTOR(nsProperties)

NS_GENERIC_FACTORY_CONSTRUCTOR_INIT(nsUUIDGenerator, Init)

#ifdef MOZ_WIDGET_COCOA
NS_GENERIC_FACTORY_CONSTRUCTOR(nsMacUtilsImpl)
#endif

NS_GENERIC_FACTORY_CONSTRUCTOR_INIT(nsSystemInfo, Init)

NS_GENERIC_FACTORY_CONSTRUCTOR_INIT(nsMemoryReporterManager, Init)

NS_GENERIC_FACTORY_CONSTRUCTOR(nsMemoryInfoDumper)

NS_GENERIC_FACTORY_CONSTRUCTOR_INIT(nsStatusReporterManager, Init)

NS_GENERIC_FACTORY_CONSTRUCTOR(nsIOUtil)

NS_GENERIC_FACTORY_CONSTRUCTOR(nsSecurityConsoleMessage)

static nsresult nsThreadManagerGetSingleton(nsISupports* aOuter,
		const nsIID& aIID, void** aInstancePtr) {
	NS_ASSERTION(aInstancePtr, "null outptr");
	if (NS_WARN_IF(aOuter)) {
		return NS_ERROR_NO_AGGREGATION;
	}

	return nsThreadManager::get()->QueryInterface(aIID, aInstancePtr);
}

NS_GENERIC_FACTORY_CONSTRUCTOR(nsThreadPool)

static nsresult nsXPTIInterfaceInfoManagerGetSingleton(nsISupports* aOuter,
		const nsIID& aIID, void** aInstancePtr) {
	NS_ASSERTION(aInstancePtr, "null outptr");
	if (NS_WARN_IF(aOuter)) {
		return NS_ERROR_NO_AGGREGATION;
	}

	nsCOMPtr<nsIInterfaceInfoManager> iim(
			XPTInterfaceInfoManager::GetSingleton());
	if (!iim) {
		return NS_ERROR_FAILURE;
	}

	return iim->QueryInterface(aIID, aInstancePtr);
}

nsComponentManagerImpl* nsComponentManagerImpl::gComponentManager = nullptr;
bool gXPCOMShuttingDown = false;
bool gXPCOMThreadsShutDown = false;
char16_t* gGREBinPath = nullptr;

static NS_DEFINE_CID(kComponentManagerCID, NS_COMPONENTMANAGER_CID);

#define COMPONENT(NAME, Ctor) static NS_DEFINE_CID(kNS_##NAME##_CID, NS_##NAME##_CID);
#include "XPCOMModule.inc"
#undef COMPONENT

#define COMPONENT(NAME, Ctor) { &kNS_##NAME##_CID, false, nullptr, Ctor },
const mozilla::Module::CIDEntry kXPCOMCIDEntries[] = { { &kComponentManagerCID,
		true, nullptr, nsComponentManagerImpl::Create },
#include "XPCOMModule.inc"
		{ nullptr } };
#undef COMPONENT

#define COMPONENT(NAME, Ctor) { NS_##NAME##_CONTRACTID, &kNS_##NAME##_CID },
const mozilla::Module::ContractIDEntry kXPCOMContracts[] = {
#include "XPCOMModule.inc"
		{ nullptr } };
#undef COMPONENT

const mozilla::Module kXPCOMModule = { mozilla::Module::kVersion,
		kXPCOMCIDEntries, kXPCOMContracts };

void moz2d_services_init_services() {

	moz2d_services_init_atom_table();
	mozilla::LogModule::Init();
	// Create the Component/Service Manager
	nsComponentManagerImpl::gComponentManager = new nsComponentManagerImpl();
	NS_ADDREF(nsComponentManagerImpl::gComponentManager);
	nsComponentManagerImpl::gComponentManager->Init();
	// Notify observers of xpcom autoregistration start
	NS_CreateServicesFromCategory(NS_XPCOM_STARTUP_CATEGORY, nullptr,
			NS_XPCOM_STARTUP_OBSERVER_ID);

	Preferences::GetInstanceForService();
}

void moz2d_services_shutdown_services() {
	Preferences::Shutdown();
	// Notify observers of xpcom shutting down
	{
		// Block it so that the COMPtr will get deleted before we hit
		// servicemanager shutdown
		RefPtr<nsObserverService> observerService;
		CallGetService("@mozilla.org/observer-service;1",
				(nsObserverService**) getter_AddRefs(observerService));

		if (observerService) {
			observerService->NotifyObservers(nullptr,
					NS_XPCOM_WILL_SHUTDOWN_OBSERVER_ID, nullptr);

			nsCOMPtr<nsIServiceManager> mgr;
			nsresult rv = NS_GetServiceManager(getter_AddRefs(mgr));
			if (NS_SUCCEEDED(rv)) {
				observerService->NotifyObservers(mgr,
						NS_XPCOM_SHUTDOWN_OBSERVER_ID, nullptr);
			}
		}

		// This must happen after the shutdown of media and widgets, which
		// are triggered by the NS_XPCOM_SHUTDOWN_OBSERVER_ID notification.
		gfxPlatform::ShutdownLayersIPC();

		if (observerService) {
			observerService->NotifyObservers(nullptr,
					NS_XPCOM_SHUTDOWN_THREADS_OBSERVER_ID, nullptr);
		}
	}
	// XPCOM is officially in shutdown mode NOW
	// Set this only after the observers have been notified as this
	// will cause servicemanager to become inaccessible.
	mozilla::services::Shutdown();
	// Shutdown global servicemanager
	if (nsComponentManagerImpl::gComponentManager) {
		nsComponentManagerImpl::gComponentManager->FreeServices();
	}
	// Finally, release the component manager last because it unloads the
	// libraries:
	if (nsComponentManagerImpl::gComponentManager) {
		nsrefcnt cnt;
		NS_RELEASE2(nsComponentManagerImpl::gComponentManager, cnt);
		NS_ASSERTION(cnt == 0,
				"Component Manager being held past XPCOM shutdown.");
	}
	nsComponentManagerImpl::gComponentManager = nullptr;
	nsCategoryManager::Destroy();

	moz2d_services_shutdown_atom_table();
}

void moz2d_services_init_platform() {
	gfxPlatform::GetPlatform();
}

void moz2d_services_shutdown_platform() {
	gfxPlatform::Shutdown();
}

void moz2d_services_init_atom_table() {
	NS_InitAtomTable();
	nsGkAtoms::AddRefAtoms();
}

void moz2d_services_shutdown_atom_table() {
	NS_ShutdownAtomTable();
}

void moz2d_services_init_gfx_config() {
	mozilla::gfx::gfxConfig::Init();
}

void moz2d_services_shutdown_gfx_config() {
	mozilla::gfx::gfxConfig::Shutdown();
}
