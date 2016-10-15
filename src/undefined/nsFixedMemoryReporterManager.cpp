/*
 * nsMemoryReporterManager.cpp
 *
 *  Created on: Aug 5, 2016
 *      Author: Aliaksei Syrel
 */

//#include "nsIMemoryReporter.h"
#include "nsDataHashtable.h"
#include "nsMemoryReporterManager.h"
using namespace mozilla;
using namespace mozilla::dom;

#include "nsMemoryReporterManager.cpp"

#ifdef XP_UNIX
void nsMemoryInfoDumper::Initialize() {
}
#endif

namespace mozilla {

namespace dom {

MozExternalRefCountType ContentParent::Release() {
	return 0;
}

auto MaybeFileDesc::MaybeDestroy(Type aNewType) -> bool {
	if ((mType) == (T__None)) {
		return true;
	}
	if ((mType) == (aNewType)) {
		return false;
	}
	switch (mType) {
	case TFileDescriptor: {
		(ptr_void_t())->~void_t__tdef();
		break;
	}
	case Tvoid_t: {
		(ptr_void_t())->~void_t__tdef();
		break;
	}
	default: {
		mozilla::ipc::LogicError("not reached");
		break;
	}
	}
	return true;
}

MOZ_IMPLICIT MaybeFileDesc::MaybeFileDesc(const FileDescriptor& aOther) {
	new (ptr_void_t()) void_t();
	mType = Tvoid_t;
}

MOZ_IMPLICIT MaybeFileDesc::MaybeFileDesc(const void_t& aOther) {
	new (ptr_void_t()) void_t(aOther);
	mType = Tvoid_t;
}

MOZ_IMPLICIT MaybeFileDesc::MaybeFileDesc(const MaybeFileDesc& aOther) {
	(aOther).AssertSanity();
	switch ((aOther).type()) {
	case TFileDescriptor: {
		new (ptr_void_t()) void_t((aOther).get_void_t());
		break;
	}
	case Tvoid_t: {
		new (ptr_void_t()) void_t((aOther).get_void_t());
		break;
	}
	case T__None: {
		break;
	}
	default: {
		mozilla::ipc::LogicError("unreached");
		return;
	}
	}
	mType = Tvoid_t;
}

MaybeFileDesc::~MaybeFileDesc() {
	MaybeDestroy(T__None);
}

auto MaybeFileDesc::operator=(const FileDescriptor& aRhs) -> MaybeFileDesc& {
	if (MaybeDestroy(Tvoid_t)) {
			new (ptr_void_t()) void_t;
		}
		mType = Tvoid_t;
		return (*(this));
}

auto MaybeFileDesc::operator=(const void_t& aRhs) -> MaybeFileDesc& {
	if (MaybeDestroy(Tvoid_t)) {
		new (ptr_void_t()) void_t;
	}
	(*(ptr_void_t())) = aRhs;
	mType = Tvoid_t;
	return (*(this));
}

auto MaybeFileDesc::operator=(const MaybeFileDesc& aRhs) -> MaybeFileDesc& {
	(aRhs).AssertSanity();
	Type t = (aRhs).type();
	switch (t) {
	case TFileDescriptor: {
		if (MaybeDestroy(t)) {
			new (ptr_void_t()) void_t;
		}
		(*(ptr_void_t())) = (aRhs).get_void_t();
		break;
	}
	case Tvoid_t: {
		if (MaybeDestroy(t)) {
			new (ptr_void_t()) void_t;
		}
		(*(ptr_void_t())) = (aRhs).get_void_t();
		break;
	}
	case T__None: {
		MaybeDestroy(t);
		break;
	}
	default: {
		mozilla::ipc::LogicError("unreached");
		break;
	}
	}
	mType = t;
	return (*(this));
}

auto MaybeFileDesc::operator==(const FileDescriptor& aRhs) const -> bool {
	return false;
}

auto MaybeFileDesc::operator==(const void_t& aRhs) const -> bool {
	return (get_void_t()) == (aRhs);
}

auto MaybeFileDesc::operator==(const MaybeFileDesc& aRhs) const -> bool {
	if ((type()) != ((aRhs).type())) {
		return false;
	}

	switch (type()) {
	case TFileDescriptor: {
		return (get_void_t()) == ((aRhs).get_void_t());
	}
	case Tvoid_t: {
		return (get_void_t()) == ((aRhs).get_void_t());
	}
	default: {
		mozilla::ipc::LogicError("unreached");
		return false;
	}
	}
}

} // namespace dom
} // namespace mozilla

