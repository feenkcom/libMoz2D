/*
 * ref_ptr.cpp
 *
 *  Created on: Aug 22, 2016
 *      Author: Aliaksei Syrel
 */

#include "ref_ptr.h"

void moz2d_plain_ptr_release(AnyPlainRefClass* refPtr) {
	delete refPtr;
}

bool moz2d_plain_ptr_is_valid(AnyPlainRefClass* refPtr) {
	return (refPtr) ? true : false;
}

MozRefCountType moz2d_ref_ptr_count(AnyRefCountedClass* refPtr) {
	return refPtr->refCount();
}

void moz2d_ref_ptr_release(AnyRefCountedClass* refPtr) {
	refPtr->Release();
}

void moz2d_ref_ptr_add_ref(AnyRefCountedClass* refPtr) {
	refPtr->AddRef();
}

void moz2d_inline_ref_ptr_release(AnyInlineRefCountedClass* refPtr) {
	refPtr->Release();
}

void moz2d_inline_ref_ptr_add_ref(AnyInlineRefCountedClass* refPtr) {
	refPtr->AddRef();
}
