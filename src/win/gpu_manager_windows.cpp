/*
 * gpu_manager.cpp
 *
 *  Created on: Oct 12, 2016
 *      Author: Aliaksei Syrel
 */

#include "gpu_manager.h"
#include "mozilla/RefPtr.h"
#include "nsWindowsHelpers.h"
#include "nsString.h"
#include <d3d11.h>

namespace mozilla {
	template class MozGpuManager<DXGI_ADAPTER_DESC>;

	const char* MozGpuManager<DXGI_ADAPTER_DESC>::GetDescription(size_t anIndex) {
		return NS_LossyConvertUTF16toASCII(nsString(mDescriptions[anIndex].Description)).get();
	}

	unsigned int  MozGpuManager<DXGI_ADAPTER_DESC>::GetDeviceId(size_t anIndex) {
		return mDescriptions[anIndex].DeviceId;
	}

	unsigned int  MozGpuManager<DXGI_ADAPTER_DESC>::GetVendorId(size_t anIndex) {
		return mDescriptions[anIndex].VendorId;
	}

	void MozGpuManager<DXGI_ADAPTER_DESC>::ReadDescriptions() {
		mDescriptions.clear();
		nsModuleHandle dxgiModule(LoadLibrarySystem32(L"dxgi.dll"));
		decltype(CreateDXGIFactory1)* createDXGIFactory1 = (decltype(CreateDXGIFactory1)*)
			GetProcAddress(dxgiModule, "CreateDXGIFactory1");

		if (!createDXGIFactory1) {
			return;
		}

		RefPtr<IDXGIFactory1> factory1;
		HRESULT hr = createDXGIFactory1(__uuidof(IDXGIFactory1), getter_AddRefs(factory1));
		if (FAILED(hr) || !factory1) {
			// This seems to happen with some people running the iZ3D driver.
			// They won't get acceleration.
			return;
		}

		for (UINT index = 0; ; index++) {
			RefPtr<IDXGIAdapter1> adapter;
			if (FAILED(factory1->EnumAdapters1(index, getter_AddRefs(adapter)))) {
				break;
			}

			DXGI_ADAPTER_DESC desc;
			if (SUCCEEDED(adapter->GetDesc(&desc)))
			{
				mDescriptions.push_back(desc);
			}
		}
	}
}

mozilla::MozGpuManager<void*>* moz_gpu_manager_create() {
	RefPtr<mozilla::MozGpuManager<DXGI_ADAPTER_DESC>> manager = new mozilla::MozGpuManager<DXGI_ADAPTER_DESC>();
	return reinterpret_cast<mozilla::MozGpuManager<void*>*>(manager.forget().take());
}

size_t moz_gpu_manager_gpu_count(mozilla::MozGpuManager<void*>* aGpuManager) {
	return reinterpret_cast<mozilla::MozGpuManager<DXGI_ADAPTER_DESC>*>(aGpuManager)->GetSize();
}

const char* moz_gpu_manager_gpu_description(mozilla::MozGpuManager<void*>* aGpuManager, size_t anIndex) {
	return reinterpret_cast<mozilla::MozGpuManager<DXGI_ADAPTER_DESC>*>(aGpuManager)->GetDescription(anIndex);
}

unsigned int moz_gpu_manager_gpu_vendor_id(mozilla::MozGpuManager<void*>* aGpuManager, size_t anIndex) {
	return reinterpret_cast<mozilla::MozGpuManager<DXGI_ADAPTER_DESC>*>(aGpuManager)->GetVendorId(anIndex);
}

unsigned int moz_gpu_manager_gpu_device_id(mozilla::MozGpuManager<void*>* aGpuManager, size_t anIndex) {
	return reinterpret_cast<mozilla::MozGpuManager<DXGI_ADAPTER_DESC>*>(aGpuManager)->GetDeviceId(anIndex);
}