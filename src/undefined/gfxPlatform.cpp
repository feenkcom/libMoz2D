/*
 * gfxPlatform.cpp
 *
 *  Created on: Aug 6, 2016
 *      Author: Aliaksei Syrel
 */

#include "VRManagerChild.h"
#include "ISurfaceAllocator.h"

namespace mozilla {
namespace layers {
void ShutdownTileCache() {}

NS_IMPL_ISUPPORTS(GfxMemoryImageReporter, nsIMemoryReporter)
mozilla::Atomic<ptrdiff_t> GfxMemoryImageReporter::sAmount(0);

} // namespace layers

namespace gfx {

/*static*/ void
VRManager::ManagerInit()
{

}

} // namespace gfx

} // namespace mozilla

