/*
 * ref_ptr.h
 *
 *  Created on: Aug 22, 2016
 *      Author: Aliaksei Syrel
 */

#ifndef REF_PTR_H_
#define REF_PTR_H_

#include "mozilla/RefPtr.h"
#include "2d/2D.h"

/**
 * We can pass any class that subclasses RefCounded
 */
class AnyClass : public mozilla::RefCounted<AnyClass>
{
public:
  MOZ_DECLARE_REFCOUNTED_VIRTUAL_TYPENAME(AnyClass)
  virtual ~AnyClass();

protected:
  AnyClass();
};

#ifdef __cplusplus
extern "C" {
#endif

MozRefCountType moz2d_ref_ptr_count(AnyClass* refPtr);
void moz2d_ref_ptr_release(AnyClass* refPtr);

#ifdef __cplusplus
}
#endif


#endif /* REF_PTR_H_ */
