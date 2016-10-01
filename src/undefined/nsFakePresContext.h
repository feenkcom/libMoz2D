/*
 * nsFakePresContext.h
 *
 *  Created on: Sep 13, 2016
 *      Author: Aliaksei Syrel
 */

#ifndef NSFAKEPRESCONTEXT_H_
#define NSFAKEPRESCONTEXT_H_


#include "2d/2D.h"
#include "mozilla/RefPtr.h"

class nsFakePresContext : public mozilla::RefCounted<nsFakePresContext>
{
public:
  MOZ_DECLARE_REFCOUNTED_VIRTUAL_TYPENAME(nsFakePresContext)
  virtual ~nsFakePresContext();
  void* GetTextPerfMetrics() {
  	return nullptr;
  }

protected:
  nsFakePresContext();
};


#endif /* NSFAKEPRESCONTEXT_H_ */
