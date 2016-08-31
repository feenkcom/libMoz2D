/*
 * Extracted from:
 * js/src/vm/TypedArrayObject.cpp
 * js/src/jsapi.h
 * dom/bindings/BindingUtils.cpp
 *
 *  Created on: Aug 1, 2016
 */

#include "stub.h"

#include "jsapi.h"
#include "jsfriendapi.h"
#include "ErrorResult.h"

using namespace js;

#define STUB(X) throw std::runtime_error (ToNewUTF8String(NS_LITERAL_STRING("[") + NS_LITERAL_STRING(X) + NS_LITERAL_STRING("] Stub method! Must not be called during runtime.")));

JS_FRIEND_API(uint8_t*)
JS_GetUint8ArrayData(JSObject* obj, bool* isSharedMemory, const JS::AutoCheckCannotGC&)
{
	STUB("JS_GetUint8ArrayData");
	return nullptr;
}

JS_FRIEND_API(JSObject*)
JS_NewUint8Array(JSContext* cx, uint32_t nelements)
{
	STUB("JS_NewUint8Array");
	return nullptr;
}

JS_FRIEND_API(void)
js::GetArrayBufferLengthAndData(JSObject* obj, uint32_t* length, bool* isSharedMemory, uint8_t** data)
{
	STUB("js::GetArrayBufferLengthAndData");
}

JS_FRIEND_API(void)
js::GetArrayBufferViewLengthAndData(JSObject* obj, uint32_t* length, bool* isSharedMemory, uint8_t** data)
{
	STUB("js::GetArrayBufferViewLengthAndData");
}

namespace mozilla {
namespace dom {

const JSErrorFormatString ErrorFormatString[] = {
#define MSG_DEF(_name, _argc, _exn, _str) \
		{ #_name, _str, _argc, _exn },
#include "mozilla/dom/Errors.msg"
#undef MSG_DEF
};

#define MSG_DEF(_name, _argc, _exn, _str) \
		static_assert(_argc < JS::MaxNumErrorArguments, \
				#_name " must only have as many error arguments as the JS engine can support");
#include "mozilla/dom/Errors.msg"
#undef MSG_DEF

const JSErrorFormatString*
GetErrorMessage(void* aUserRef, const unsigned aErrorNumber)
{
	MOZ_ASSERT(aErrorNumber < ArrayLength(ErrorFormatString));
	return &ErrorFormatString[aErrorNumber];
}

uint16_t
GetErrorArgCount(const ErrNum aErrorNumber)
{
	return GetErrorMessage(nullptr, aErrorNumber)->argCount;
}

} // namespace dom

namespace binding_danger {

template<typename CleanupPolicy>
void
TErrorResult<CleanupPolicy>::SuppressException()
{
	STUB("TErrorResult<CleanupPolicy>::SuppressException");
}

template<typename CleanupPolicy>
void
TErrorResult<CleanupPolicy>::SetPendingException(JSContext* cx)
{
	STUB("TErrorResult<CleanupPolicy>::SetPendingException");
}

template<typename CleanupPolicy>
void
TErrorResult<CleanupPolicy>::ClearUnionData()
{
	STUB("TErrorResult<CleanupPolicy>::ClearUnionData");
}

template<typename CleanupPolicy>
nsTArray<nsString>&
TErrorResult<CleanupPolicy>::CreateErrorMessageHelper(const dom::ErrNum errorNumber,
                                                      nsresult errorType)
{
	STUB("TErrorResult<CleanupPolicy>::CreateErrorMessageHelper");
}

template<typename CleanupPolicy>
TErrorResult<CleanupPolicy>&
TErrorResult<CleanupPolicy>::operator=(TErrorResult<CleanupPolicy>&& aRHS)
{
	STUB("TErrorResult<CleanupPolicy>::operator=");
}

template class TErrorResult<AssertAndSuppressCleanupPolicy>;

} // namespace binding_danger
} // namespace mozilla

// Needed by mozilla::dom::TextEncoder::Encode
JSAutoCompartment::JSAutoCompartment(JSContext* cx, JSObject* target MOZ_GUARD_OBJECT_NOTIFIER_PARAM) {
	cx_ = nullptr;
	oldCompartment_ = nullptr;
	STUB("JSAutoCompartment");
}
// Needed by mozilla::dom::TextEncoder::Encode
JSAutoCompartment::JSAutoCompartment(JSContext* cx, JSScript* target MOZ_GUARD_OBJECT_NOTIFIER_PARAM) {
	cx_ = nullptr;
	oldCompartment_ = nullptr;
	STUB("JSAutoCompartment");
}
// Needed by mozilla::dom::TextEncoder::Encode
JSAutoCompartment::~JSAutoCompartment() {
	STUB("~JSAutoCompartment");}

bool
JS::GCCellPtr::mayBeOwnedByOtherRuntime() const
{
    return false;
}
