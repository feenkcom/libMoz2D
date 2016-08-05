/*
 * stub.h
 *
 *  Created on: Aug 1, 2016
 *      Author: Aliaksei Syrel
 */

#ifndef STUB_H_
#define STUB_H_

#include <string.h>
#include <stdexcept>
#include "nsString.h"

#define STUB(X) throw std::runtime_error (ToNewUTF8String(NS_LITERAL_STRING("[") + NS_LITERAL_STRING(X) + NS_LITERAL_STRING("] Stub method! Must not be called during runtime.")));


#endif /* STUB_H_ */
