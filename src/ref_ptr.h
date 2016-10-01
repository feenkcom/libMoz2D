/*
 * ref_ptr.h
 *
 *  Created on: Aug 22, 2016
 *      Author: Aliaksei Syrel
 */

#ifndef REF_PTR_H_
#define REF_PTR_H_

#include "mozilla/RefPtr.h"
#include "nsISupports.h"
#include "mozilla/RefCounted.h"

/**
 * We can pass any class that subclasses RefCounded
 */
class AnyRefCountedClass : public mozilla::RefCounted<AnyRefCountedClass>
{
public:
  MOZ_DECLARE_REFCOUNTED_VIRTUAL_TYPENAME(AnyRefCountedClass)
  virtual ~AnyRefCountedClass();

protected:
  AnyRefCountedClass();
};

/**
 * We can pass any class that adds inlined support of ref counting
 */
class AnyInlineRefCountedClass
{
	NS_INLINE_DECL_REFCOUNTING(AnyInlineRefCountedClass)
public:
	MozRefCountType refCount(){ return mRefCnt; }
protected:
    // Protected destructor, to discourage deletion outside of Release():
    virtual ~AnyInlineRefCountedClass() {}
};

/**
 * Objects without support of smart pointer should be released using delete
 */
class AnyPlainRefClass
{

};

#ifdef __cplusplus
extern "C" {
#endif

void moz2d_plain_ptr_release(AnyPlainRefClass* refPtr);
bool moz2d_plain_ptr_is_valid(AnyPlainRefClass* refPtr);

MozRefCountType moz2d_ref_ptr_count(AnyRefCountedClass* refPtr);
void moz2d_ref_ptr_add_ref(AnyRefCountedClass* refPtr);
void moz2d_ref_ptr_release(AnyRefCountedClass* refPtr);

MozRefCountType moz2d_inline_ref_ptr_count(AnyInlineRefCountedClass* refPtr);
void moz2d_inline_ref_ptr_release(AnyInlineRefCountedClass* refPtr);
void moz2d_inline_ref_ptr_add_ref(AnyInlineRefCountedClass* refPtr);

#ifdef __cplusplus
}
#endif


#endif /* REF_PTR_H_ */
