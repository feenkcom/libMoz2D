/*
 * gpu_manager_unix.cpp
 *
 *  Created on: Oct 14, 2016
 *      Author: Aliaksei Syrel
 */

#include "gpu_manager.h"
#include "mozilla/RefPtr.h"
#include "nsString.h"

namespace mozilla {

	template<> const char* MozGpuManager<void*>::GetDescription(size_t anIndex) {
		return NS_LITERAL_CSTRING("").get();
	}

	template<> unsigned int MozGpuManager<void*>::GetDeviceId(size_t anIndex) {
		return 0;
	}

	template<> unsigned int  MozGpuManager<void*>::GetVendorId(size_t anIndex) {
		return 0;
	}

	template<> void MozGpuManager<void*>::ReadDescriptions() {
		mDescriptions.clear();
	}

	template class MozGpuManager<void*>;
}

mozilla::MozGpuManager<void*>* moz_gpu_manager_create() {
	RefPtr<mozilla::MozGpuManager<void*>> manager = new mozilla::MozGpuManager<void*>();
	return reinterpret_cast<mozilla::MozGpuManager<void*>*>(manager.forget().take());
}

size_t moz_gpu_manager_gpu_count(mozilla::MozGpuManager<void*>* aGpuManager) {
	return reinterpret_cast<mozilla::MozGpuManager<void*>*>(aGpuManager)->GetSize();
}

const char* moz_gpu_manager_gpu_description(mozilla::MozGpuManager<void*>* aGpuManager, size_t anIndex) {
	return reinterpret_cast<mozilla::MozGpuManager<void*>*>(aGpuManager)->GetDescription(anIndex);
}

unsigned int moz_gpu_manager_gpu_vendor_id(mozilla::MozGpuManager<void*>* aGpuManager, size_t anIndex) {
	return reinterpret_cast<mozilla::MozGpuManager<void*>*>(aGpuManager)->GetVendorId(anIndex);
}

unsigned int moz_gpu_manager_gpu_device_id(mozilla::MozGpuManager<void*>* aGpuManager, size_t anIndex) {
	return reinterpret_cast<mozilla::MozGpuManager<void*>*>(aGpuManager)->GetDeviceId(anIndex);
}
