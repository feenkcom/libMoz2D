/*
 * gpu_manager.h
 *
 *  Created on: Oct 12, 2016
 *      Author: Aliaksei Syrel
 */

#ifndef GPU_MANAGER_H_
#define GPU_MANAGER_H_

#include "exports.h"
#include "mozilla/GenericRefCounted.h"
#include <vector>

namespace mozilla {
	template<class T> class MozGpuManager : public RefCounted<MozGpuManager<T>>
	{
	public:
		MOZ_DECLARE_REFCOUNTED_VIRTUAL_TYPENAME(MozGpuManager<T>)
		virtual ~MozGpuManager() { }
		MozGpuManager() { ReadDescriptions(); }

		size_t GetSize() { return mDescriptions.size(); };

		const char* GetDescription(size_t anIndex);
		unsigned int GetVendorId(size_t anIndex);
		unsigned int GetDeviceId(size_t anIndex);
	private:
		void ReadDescriptions();
		std::vector<T> mDescriptions;
	};
}

#ifdef __cplusplus
extern "C" {
#endif

	LIBRARY_API mozilla::MozGpuManager<void*>* moz_gpu_manager_create();
	LIBRARY_API size_t moz_gpu_manager_gpu_count(mozilla::MozGpuManager<void*>* aGpuManager);
	LIBRARY_API const char* moz_gpu_manager_gpu_description(mozilla::MozGpuManager<void*>* aGpuManager, size_t anIndex);
	LIBRARY_API unsigned int moz_gpu_manager_gpu_vendor_id(mozilla::MozGpuManager<void*>* aGpuManager, size_t anIndex);
	LIBRARY_API unsigned int moz_gpu_manager_gpu_device_id(mozilla::MozGpuManager<void*>* aGpuManager, size_t anIndex);

#ifdef __cplusplus
}
#endif

#endif /* GPU_MANAGER_H_ */
