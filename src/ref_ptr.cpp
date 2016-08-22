/*
 * ref_ptr.cpp
 *
 *  Created on: Aug 22, 2016
 *      Author: Aliaksei Syrel
 */

#include "ref_ptr.h"

MozRefCountType moz2d_ref_ptr_count(AnyClass* refPtr) {
	return refPtr->refCount();
}

void moz2d_ref_ptr_release(AnyClass* refPtr) {
	refPtr->Release();
}
