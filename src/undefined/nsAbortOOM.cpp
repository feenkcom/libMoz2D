/*
 * nsAbortOOM.cpp
 *
 *  Created on: Oct 11, 2016
 *      Author: Aliaksei Syrel
 */

#include "stub.h"


#if defined(MOZILLA_INTERNAL_API) || defined(USE_NS_ABORT_OOM)
void
NS_ABORT_OOM(size_t aSize)
{
  STUB("NS_ABORT_OOM");
}
#endif