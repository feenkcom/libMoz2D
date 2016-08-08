/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/MemoryReporting.h"

#include "mozilla/ArrayUtils.h"
#include "mozilla/Attributes.h"
#include "mozilla/HashFunctions.h"
#include "mozilla/UniquePtrExtensions.h"

#include "nsXULAppAPI.h"

#include "mozilla/Preferences.h"
#include "nsAppDirectoryServiceDefs.h"
#include "nsDataHashtable.h"
#include "nsDirectoryServiceDefs.h"
#include "nsICategoryManager.h"
#include "nsCategoryManagerUtils.h"
#include "nsNetUtil.h"
#include "nsIFile.h"
#include "nsIInputStream.h"
#include "nsIObserverService.h"
#include "nsIOutputStream.h"
#include "nsISafeOutputStream.h"
#include "nsISimpleEnumerator.h"
#include "nsIStringEnumerator.h"
#include "nsIZipReader.h"
#include "nsXPIDLString.h"
#include "nsCRT.h"
#include "nsCOMArray.h"
#include "nsXPCOMCID.h"
#include "nsAutoPtr.h"
#include "nsPrintfCString.h"

#include "nsQuickSort.h"
#include "PLDHashTable.h"

#include "mozilla/Omnijar.h"
#include "nsZipArchive.h"

#include "nsTArray.h"
#include "nsRefPtrHashtable.h"
#include "nsIMemoryReporter.h"
#include "nsThreadUtils.h"

#include "nsString.h"

#include <iostream> // For std::cout
//#include <new>

namespace mozilla {


static bool DEBUG = true;
#define LOG(X) if (DEBUG) std::cout << X << "\n";


enum {
	eNoType = 0,
	eBooleanType = 1,
	eLongType = 2,
	eDoubleType = 3,
	eWStringType = 4,
	eISupportsType = 5,
	eStringType = 6
};
struct HashEntry : public PLDHashEntryHdr
{
	nsCString mEntryName;

	uint8_t mEntryType;
	union
	{
		int32_t mLong;
		nsCString* mCString;
	} mData;

	HashEntry(uint8_t aType, const char* aEntryName)
	: mEntryName(aEntryName)
	, mEntryType(aType)
	{
		Reset(mEntryType);
	}

	HashEntry(const HashEntry& aRHS)
	: mEntryType(aRHS.mEntryType)
	{
		Reset(mEntryType);
		switch (mEntryType) {
		case eLongType:
			mData.mLong = aRHS.mData.mLong;
			break;
		case eStringType:
			NS_ASSERTION(aRHS.mData.mCString, "Source entry has no string");
			mData.mCString = new nsCString(*aRHS.mData.mCString);
			break;
		default:
			NS_ERROR("Unknown type");
		}
	}

	~HashEntry() {
		LOG("[HashEntry] Destroyed " << (mEntryName.get()));
		Reset(eNoType); }

	void Reset(uint8_t aNewType)
	{
		switch (mEntryType) {
		case eNoType:
			break;
		case eLongType:
			mData.mLong = 0;
			break;
		case eStringType:
			delete mData.mCString;
			mData.mCString = nullptr;
			break;
		default:
			NS_ERROR("Unknown type");
		}
		mEntryType = aNewType;
	}
};

void
Preferences::DirtyCallback()
{
	if (sPreferences && !sPreferences->mDirty) {
		sPreferences->mDirty = true;
	}
}

Preferences* Preferences::sPreferences = nullptr;
nsIPrefBranch* Preferences::sRootBranch = nullptr;
bool Preferences::sShutdown = false;
static PLDHashTable* table;

/*
 * Constructor/Destructor
 */

static PLDHashTable* GetTable() {
	return table;
}

static HashEntry* GetNamedEntry(const char* aName)
{
	Preferences::ResetAndReadUserPrefs();
	return static_cast<HashEntry*>(table->Search((void*)aName));
}

static HashEntry* GetOrMakeEntry(const char* aName, uint8_t aEntryType)
{
	Preferences::ResetAndReadUserPrefs();
	auto foundEntry = static_cast<HashEntry*>(table->Search((void*)aName));
	if (foundEntry) { // reuse existing entry
		LOG("-------------- REWRITE -------------")
		foundEntry->Reset(aEntryType);
		return foundEntry;
	}

	foundEntry = static_cast<HashEntry*>(table->Add((void*)aName, fallible));
	if (!foundEntry) {
		LOG("[Pref] Adding new entry failed!")
		return nullptr;
	}

	// Use placement new. Our actor does not clobber keyHash, which is important.
	new (foundEntry) HashEntry(aEntryType, aName);
	return foundEntry;
}

static void pref_bool(const char* aName, bool aValue) {
	LOG("[Init   Bool] "<< aName << ": " << (aValue ? "true" : "false"));
	HashEntry* foundEntry = GetOrMakeEntry(aName, eLongType);
	if (!foundEntry) { return; }
	foundEntry->mData.mLong = aValue;
	LOG(table->EntryCount());
}
static void pref_string(const char* aName, const char*  aValue) {
	LOG("[Init String] "<< aName << ": " << "\"" << aValue << "\"");
	HashEntry* foundEntry = GetOrMakeEntry(aName, eStringType);
	if (!foundEntry) { return; }
	foundEntry->mData.mCString = new nsCString(aValue);
	LOG(table->EntryCount());
}
static void pref_long(const char* aName, int32_t aValue) {
	LOG("[Init   Long] "<< aName << ": " << aValue);
	HashEntry* foundEntry = GetOrMakeEntry(aName, eLongType);
	if (!foundEntry) { return; }
	foundEntry->mData.mLong = aValue;
	LOG(table->EntryCount());
}
static void pref_double(const char* aName, double aValue) {
	LOG("[Init Double] "<< aName << ": " << aValue);
	HashEntry* foundEntry = GetOrMakeEntry(aName, eStringType);
	if (!foundEntry) { return; }
	foundEntry->mData.mCString = new nsCString(nsPrintfCString("%f", aValue).get());
	LOG(table->EntryCount());
}

static void pref_float(const char* aName, float aValue) {
	LOG("[Init  Float] "<< aName << ": " << aValue);
	HashEntry* foundEntry = GetOrMakeEntry(aName, eStringType);
	if (!foundEntry) { return; }
	foundEntry->mData.mCString = new nsCString(nsPrintfCString("%f", aValue).get());
	LOG(table->EntryCount());
}

static void pref_default(const char* aName, void*  aValue) {
	LOG("Unknown preference type!");
}

#define pref(x, y) _Generic((x), \
		const char[sizeof(x)]: _Generic((y), \
				bool: pref_bool, \
				double: pref_double, \
				float: pref_float, \
				const char[sizeof(y)]: pref_string, \
				const char*: pref_string, \
				int32_t: pref_long, \
				default: pref_default), \
		const char*: _Generic((y), \
				bool: pref_bool, \
				double: pref_double, \
				float: pref_float, \
				const char[sizeof(y)]: pref_string, \
				const char*: pref_string, \
				int32_t: pref_long, \
				default: pref_default) \
)(x, y)


PLDHashNumber HashKey(const void* aKey)
{
	return HashString((const char*)aKey);
}

bool HashMatchEntry(const PLDHashEntryHdr* aEntry, const void* aKey)
{
	const char* keyString = (const char*)aKey;
	const HashEntry* thisEntry = static_cast<const HashEntry*>(aEntry);
	return thisEntry->mEntryName.Equals(keyString);
}

void HashMoveEntry(PLDHashTable* aTable,
		const PLDHashEntryHdr* aFrom,
		PLDHashEntryHdr* aTo)
{
	const HashEntry* fromEntry = static_cast<const HashEntry*>(aFrom);
	HashEntry* toEntry = static_cast<HashEntry*>(aTo);

	new (toEntry) HashEntry(*fromEntry);

	fromEntry->~HashEntry();
}

void HashClearEntry(PLDHashTable* aTable, PLDHashEntryHdr* aEntry)
{
	HashEntry* thisEntry = static_cast<HashEntry*>(aEntry);
	thisEntry->~HashEntry();
}

const PLDHashTableOps sHashOps =
{
		HashKey,
		HashMatchEntry,
		HashMoveEntry,
		HashClearEntry
};

// static
Preferences*
Preferences::GetInstanceForService()
{
	if (sPreferences) {
		NS_ADDREF(sPreferences);
		return sPreferences;
	}

	NS_ENSURE_TRUE(!sShutdown, nullptr);

	sPreferences = new Preferences();
	NS_ADDREF(sPreferences);

	if (NS_FAILED(sPreferences->Init())) {
		// The singleton instance will delete sRootBranch and sDefaultRootBranch.
		NS_RELEASE(sPreferences);
		return nullptr;
	}

	NS_ADDREF(sPreferences);
	return sPreferences;
}

// static
bool
Preferences::IsServiceAvailable()
{
	return !!sPreferences;
}

// static
bool
Preferences::InitStaticMembers()
{
	return true;
}


// static
void
Preferences::Shutdown()
{
	if (!sShutdown) {
		sShutdown = true; // Don't create the singleton instance after here.

		// Don't set sPreferences to nullptr here.  The instance may be grabbed by
		// other modules.  The utility methods of Preferences should be available
		// until the singleton instance actually released.
		if (sPreferences) {
			sPreferences->Release();
		}
	}
}


// static
static void InitTable()
{
	if (table)
		return;

	table = new PLDHashTable(&sHashOps, sizeof(HashEntry), 8192);
	DEBUG = false;
	#include "all.js"
	#include "custom.js"
	DEBUG = true;

//	for (auto iter = table->Iter(); !iter.Done(); iter.Next()) {
//		auto entry = static_cast<HashEntry*>(iter.Get());
//		LOG((ToNewCString(entry->mEntryName)) << "  " << (entry->mEntryType));
//	}

}
//-----------------------------------------------------------------------------



Preferences::Preferences()
: mDirty(false)
{
	InitTable();
}


Preferences::~Preferences()
{
	NS_ASSERTION(sPreferences == this, "Isn't this the singleton instance?");

	sPreferences = nullptr;
	delete table;
	table = nullptr;
}


/*
 * nsISupports Implementation
 */

NS_IMPL_ADDREF(Preferences)
NS_IMPL_RELEASE(Preferences)

NS_INTERFACE_MAP_BEGIN(Preferences)
NS_INTERFACE_MAP_ENTRY_AMBIGUOUS(nsISupports, nsIPrefService)
NS_INTERFACE_MAP_ENTRY(nsIPrefService)
NS_INTERFACE_MAP_ENTRY(nsIObserver)
NS_INTERFACE_MAP_ENTRY(nsIPrefBranch)
NS_INTERFACE_MAP_ENTRY(nsIPrefBranch2)
NS_INTERFACE_MAP_ENTRY(nsIPrefBranchInternal)
NS_INTERFACE_MAP_ENTRY(nsISupportsWeakReference)
NS_INTERFACE_MAP_END

nsresult
Preferences::Init()
{
	InitTable();

	return NS_OK;
}

// static
nsresult
Preferences::ResetAndReadUserPrefs()
{
	InitTable();
	return NS_OK;
}

NS_IMETHODIMP
Preferences::Observe(nsISupports *aSubject, const char *aTopic,
		const char16_t *someData)
{
	return NS_OK;
}


NS_IMETHODIMP
Preferences::ReadUserPrefs(nsIFile *aFile)
{
	return NS_OK;
}

NS_IMETHODIMP
Preferences::ResetPrefs()
{
	return NS_OK;
}

NS_IMETHODIMP
Preferences::ResetUserPrefs()
{
	return NS_OK;
}

NS_IMETHODIMP
Preferences::SavePrefFile(nsIFile *aFile)
{
	return NS_OK;
}

static nsresult
ReadExtensionPrefs(nsIFile *aFile)
{
	return NS_OK;
}

void
Preferences::SetPreference(const PrefSetting& aPref)
{

}

void
Preferences::GetPreference(PrefSetting* aPref)
{

}

void
Preferences::GetPreferences(InfallibleTArray<PrefSetting>* aPrefs)
{

}

NS_IMETHODIMP
Preferences::GetBranch(const char *aPrefRoot, nsIPrefBranch **_retval)
{
	return NS_OK;
}

NS_IMETHODIMP
Preferences::GetDefaultBranch(const char *aPrefRoot, nsIPrefBranch **_retval)
{
	return NS_OK;
}

NS_IMETHODIMP
Preferences::GetDirty(bool *_retval) {
	*_retval = mDirty;
	return NS_OK;
}

nsresult
Preferences::NotifyServiceObservers(const char *aTopic)
{
	return NS_OK;
}

nsresult
Preferences::UseDefaultPrefFile()
{
	return NS_OK;
}

nsresult
Preferences::UseUserPrefFile()
{
	return NS_OK;
}

nsresult
Preferences::MakeBackupPrefFile(nsIFile *aFile)
{
	return NS_OK;
}

nsresult
Preferences::ReadAndOwnUserPrefFile(nsIFile *aFile)
{
	return NS_OK;
}

nsresult
Preferences::SavePrefFileInternal(nsIFile *aFile)
{
	return NS_OK;
}

nsresult
Preferences::WritePrefFile(nsIFile* aFile)
{
	return NS_OK;
}

static nsresult openPrefFile(nsIFile* aFile)
{
	return NS_OK;
}

/******************************************************************************
 *
 * static utilities
 *
 ******************************************************************************/

// static
nsresult
Preferences::GetBool(const char* aName, bool* aRetVal)
{
	NS_PRECONDITION(aResult, "aResult must not be NULL");

	HashEntry* foundEntry = GetNamedEntry(aName);
	if (foundEntry && foundEntry->mEntryType == eLongType) {
		LOG("[Pref::GetBool In] " << aName << ": " << (((bool)foundEntry->mData.mLong) ? "true" : "false"));
		*aRetVal = (foundEntry->mData.mLong);
		return NS_OK;
	}
	*aRetVal = false;
	LOG("[Pref::GetBool In] " << aName << ": " << "Not found!");
	return NS_ERROR_FAILURE;
}

// static
nsresult
Preferences::GetInt(const char* aName, int32_t* aRetVal)
{
	NS_PRECONDITION(aResult, "aResult must not be NULL");

	HashEntry* foundEntry = GetNamedEntry(aName);
	if (foundEntry && foundEntry->mEntryType == eLongType) {
		LOG("[Pref::GetInt In] " << aName << ": " << foundEntry->mData.mLong);
		*aRetVal = foundEntry->mData.mLong;
		return NS_OK;
	}
	*aRetVal = 0;
	LOG("[Pref::GetInt In] " << aName << ": " << "Not found!");
	return NS_ERROR_FAILURE;
}

// static
nsresult
Preferences::GetFloat(const char* aName, float* aRetVal)
{

	HashEntry* foundEntry = GetNamedEntry(aName);
	if (foundEntry && foundEntry->mEntryType == eStringType) {
		LOG("[Pref::GetFloat In] " << aName << ": " << (ToNewCString(*foundEntry->mData.mCString)));
		nsAdoptingCString result = nsAdoptingCString(ToNewCString(*(foundEntry->mData.mCString)));
		nsresult rv = NS_OK;
		*aRetVal = result.ToFloat(&rv);
		return rv;
	}
	*aRetVal = 0.f;
	LOG("[Pref::GetFloat In] " << aName << ": " << "Not found!");
	return NS_ERROR_FAILURE;
}

// static
nsAdoptingCString
Preferences::GetCString(const char* aName)
{
	nsAdoptingCString result;
	HashEntry* foundEntry = GetNamedEntry(aName);
	if (foundEntry && foundEntry->mEntryType == eStringType) {
		LOG("[Pref::GetCString] " << aName << ": " << (ToNewCString(*foundEntry->mData.mCString)));
		result = nsAdoptingCString(ToNewCString(*(foundEntry->mData.mCString)));
		return result;
	}
	LOG("[Pref::GetCString] " << aName << ": " << "Not found!");
	return result;
}

// static
nsAdoptingString
Preferences::GetString(const char* aName)
{
	nsAdoptingString result;
	HashEntry* foundEntry = GetNamedEntry(aName);
	if (foundEntry && foundEntry->mEntryType == eStringType) {
		LOG("[Pref::GetString] " << aName << ": " << (ToNewCString(*foundEntry->mData.mCString)));
		result = NS_ConvertUTF8toUTF16(ToNewCString(*(foundEntry->mData.mCString)));
		return result;
	}
	LOG("[Pref::GetString] " << aName << ": " << "Not found!");
	return result;
}

// static
nsresult
Preferences::GetCString(const char* aName, nsACString* aRetVal)
{
	nsAdoptingCString result;
	HashEntry* foundEntry = GetNamedEntry(aName);
	if (foundEntry && foundEntry->mEntryType == eStringType) {
		LOG("[Pref::GetCString In] " << aName << ": " << (ToNewCString(*foundEntry->mData.mCString)));
		result = nsAdoptingCString(ToNewCString(*(foundEntry->mData.mCString)));
		*aRetVal = result;
		return NS_OK;
	}
	*aRetVal = result;
	LOG("[Pref::GetCString In] " << aName << ": " << "Not found!");
	return NS_ERROR_FAILURE;
}

// static
nsresult
Preferences::GetString(const char* aName, nsAString* aRetVal)
{
	nsAdoptingString result;
	HashEntry* foundEntry = GetNamedEntry(aName);
	if (foundEntry && foundEntry->mEntryType == eStringType) {
		LOG("[Pref::GetString In] " << aName << ": " << (ToNewCString(*foundEntry->mData.mCString)));
		result = NS_ConvertUTF8toUTF16(ToNewCString(*(foundEntry->mData.mCString)));
		*aRetVal = result;
		return NS_OK;
	}
	*aRetVal = result;
	LOG("[Pref::GetString In] " << aName << ": " << "Not found!");
	return NS_ERROR_FAILURE;
}

// static
nsAdoptingCString
Preferences::GetLocalizedCString(const char* aPref)
{
	nsAdoptingCString result;

	std::cout << "[Pref::GetLocalizedCString] " << aPref << "\n";

	return result;
}

// static
nsAdoptingString
Preferences::GetLocalizedString(const char* aPref)
{
	nsAdoptingString result;

	std::cout << "[Pref::GetLocalizedString] " << aPref << "\n";

	return result;
}

// static
nsresult
Preferences::GetLocalizedCString(const char* aPref, nsACString* aResult)
{
	NS_PRECONDITION(aResult, "aResult must not be NULL");
	std::cout << "[Pref::GetLocalizedCString] " << aPref << "\n";
	return NS_OK;
}

// static
nsresult
Preferences::GetLocalizedString(const char* aPref, nsAString* aResult)
{
	NS_PRECONDITION(aResult, "aResult must not be NULL");
	std::cout << "[Pref::GetLocalizedCString] " << aPref << "\n";
	return NS_OK;
}

// static
nsresult
Preferences::GetComplex(const char* aPref, const nsIID &aType, void** aResult)
{
	std::cout << "[Pref] " << aPref << "\n";
	return NS_OK;
}

// static
nsresult
Preferences::SetCString(const char* aName, const char* aValue)
{
	LOG("[Pref::SetCString] " << aName << ": " << aValue);
	HashEntry* foundEntry = GetOrMakeEntry(aName, eStringType);
	if (!foundEntry) {
		return NS_ERROR_OUT_OF_MEMORY;
	}
	foundEntry->mData.mCString = new nsCString(aValue);
	return NS_OK;
}

// static
nsresult
Preferences::SetCString(const char* aName, const nsACString &aValue)
{
	LOG("[Pref::SetCString] " << aName << ": " << ToNewCString(aValue));
	HashEntry* foundEntry = GetOrMakeEntry(aName, eStringType);
	if (!foundEntry) {
		return NS_ERROR_OUT_OF_MEMORY;
	}
	foundEntry->mData.mCString = new nsCString(ToNewCString(aValue));
	return NS_OK;
}

// static
nsresult
Preferences::SetString(const char* aName, const char16ptr_t aValue)
{
	LOG("[Pref::SetString] " << aName << ": " << NS_LossyConvertUTF16toASCII(aValue).get());
	HashEntry* foundEntry = GetOrMakeEntry(aName, eStringType);
	if (!foundEntry) {
		return NS_ERROR_OUT_OF_MEMORY;
	}
	foundEntry->mData.mCString = new nsCString(NS_ConvertUTF16toUTF8(aValue));
	return NS_OK;
}

// static
nsresult
Preferences::SetString(const char* aName, const nsAString &aValue)
{
	LOG("[Pref::SetString] " << aName << ": " << NS_LossyConvertUTF16toASCII(aValue).get());
	HashEntry* foundEntry = GetOrMakeEntry(aName, eStringType);
	if (!foundEntry) {
		return NS_ERROR_OUT_OF_MEMORY;
	}
	foundEntry->mData.mCString = new nsCString(NS_ConvertUTF16toUTF8(aValue));
	return NS_OK;
}

// static
nsresult
Preferences::SetBool(const char* aName, bool aValue)
{

	LOG("[Pref::SetBool] " << aName << ": " << aValue);
	HashEntry* foundEntry = GetOrMakeEntry(aName, eLongType);
	if (!foundEntry) {
		return NS_ERROR_OUT_OF_MEMORY;
	}
	foundEntry->mData.mLong = aValue;
	return NS_OK;
}

// static
nsresult
Preferences::SetInt(const char* aPref, int32_t aValue)
{
	std::cout << "[Pref] " << aPref << "\n";
	return NS_OK;
}

// static
nsresult
Preferences::SetFloat(const char* aPref, float aValue)
{
	std::cout << "[Pref] " << aPref << "\n";
	return NS_OK;
}

// static
nsresult
Preferences::SetComplex(const char* aPref, const nsIID &aType,
		nsISupports* aValue)
{
	std::cout << "[Pref] " << aPref << "\n";
	return NS_OK;
}

// static
nsresult
Preferences::ClearUser(const char* aPref)
{
	std::cout << "[Pref] " << aPref << "\n";
	return NS_OK;
}

// static
bool
Preferences::HasUserValue(const char* aPref)
{
	std::cout << "[Pref] " << aPref << "\n";
	return false;
}

// static
int32_t
Preferences::GetType(const char* aPref)
{
	std::cout << "[Pref] " << aPref << "\n";
	return 0;
}

// static
nsresult
Preferences::AddStrongObserver(nsIObserver* aObserver,
		const char* aPref)
{
	std::cout << "[Pref] " << aPref << "\n";
	return NS_OK;
}

// static
nsresult
Preferences::AddWeakObserver(nsIObserver* aObserver,
		const char* aPref)
{
	std::cout << "[Pref] " << aPref << "\n";
	return NS_OK;
}

// static
nsresult
Preferences::RemoveObserver(nsIObserver* aObserver,
		const char* aPref)
{
	if (!sPreferences && sShutdown) {
		return NS_OK; // Observers have been released automatically.
	}
	std::cout << "[Pref] " << aPref << "\n";
	return NS_OK;
}

// static
nsresult
Preferences::AddStrongObservers(nsIObserver* aObserver,
		const char** aPrefs)
{
	return NS_OK;
}

// static
nsresult
Preferences::AddWeakObservers(nsIObserver* aObserver,
		const char** aPrefs)
{
	return NS_OK;
}

// static
nsresult
Preferences::RemoveObservers(nsIObserver* aObserver,
		const char** aPrefs)
{
	return NS_OK;
}

// static
nsresult
Preferences::RegisterCallback(PrefChangedFunc aCallback,
		const char* aPref,
		void* aClosure,
		MatchKind aMatchKind)
{
	std::cout << "[Pref] " << aPref << "\n";
	return NS_OK;
}

// static
nsresult
Preferences::RegisterCallbackAndCall(PrefChangedFunc aCallback,
		const char* aPref,
		void* aClosure,
		MatchKind aMatchKind)
{
	std::cout << "[Pref] " << aPref << "\n";
	return NS_OK;
}

// static
nsresult
Preferences::UnregisterCallback(PrefChangedFunc aCallback,
		const char* aPref,
		void* aClosure,
		MatchKind aMatchKind)
{
	std::cout << "[Pref] " << aPref << "\n";
	return NS_OK;
}

static void BoolVarChanged(const char* aPref, void* aClosure)
{
	std::cout << "[Pref] " << aPref << "\n";
}

// static
nsresult
Preferences::AddBoolVarCache(bool* aCache,
		const char* aPref,
		bool aDefault)
{
	NS_ASSERTION(aCache, "aCache must not be NULL");
	std::cout << "[Pref] " << aPref << "\n";
	return NS_OK;
}

static void IntVarChanged(const char* aPref, void* aClosure)
{
	std::cout << "[Pref] " << aPref << "\n";
}

// static
nsresult
Preferences::AddIntVarCache(int32_t* aCache,
		const char* aPref,
		int32_t aDefault)
{
	NS_ASSERTION(aCache, "aCache must not be NULL");
	std::cout << "[Pref] " << aPref << "\n";
	return NS_OK;
}

static void UintVarChanged(const char* aPref, void* aClosure)
{
	std::cout << "[Pref] " << aPref << "\n";
}

// static
nsresult
Preferences::AddUintVarCache(uint32_t* aCache,
		const char* aPref,
		uint32_t aDefault)
{
	std::cout << "[Pref] " << aPref << "\n";
	return NS_OK;
}

template <MemoryOrdering Order>
static void AtomicUintVarChanged(const char* aPref, void* aClosure)
{
	std::cout << "[Pref] " << aPref << "\n";
}

template <MemoryOrdering Order>
// static
nsresult
Preferences::AddAtomicUintVarCache(Atomic<uint32_t, Order>* aCache,
		const char* aPref,
		uint32_t aDefault)
{
	NS_ASSERTION(aCache, "aCache must not be NULL");
	std::cout << "[Pref] " << aPref << "\n";
	return NS_OK;
}

// Since the definition of this template function is not in a header file,
// we need to explicitly specify the instantiations that are required.
// Currently only the order=Relaxed variant is needed.
template
nsresult Preferences::AddAtomicUintVarCache(Atomic<uint32_t,Relaxed>*,
		const char*, uint32_t);

static void FloatVarChanged(const char* aPref, void* aClosure)
{
	std::cout << "[Pref] " << aPref << "\n";
}

// static
nsresult
Preferences::AddFloatVarCache(float* aCache,
		const char* aPref,
		float aDefault)
{
	NS_ASSERTION(aCache, "aCache must not be NULL");
	std::cout << "[Pref] " << aPref << "\n";
	return NS_OK;
}

// static
nsresult
Preferences::GetDefaultBool(const char* aPref, bool* aResult)
{
	NS_PRECONDITION(aResult, "aResult must not be NULL");
	std::cout << "[Pref] " << aPref << "\n";
	return NS_OK;
}

// static
nsresult
Preferences::GetDefaultInt(const char* aPref, int32_t* aResult)
{
	NS_PRECONDITION(aResult, "aResult must not be NULL");
	std::cout << "[Pref] " << aPref << "\n";
	return NS_OK;
}

// static
nsresult
Preferences::GetDefaultCString(const char* aPref, nsACString* aResult)
{
	NS_PRECONDITION(aResult, "aResult must not be NULL");
	std::cout << "[Pref] " << aPref << "\n";
	return NS_OK;
}

// static
nsresult
Preferences::GetDefaultString(const char* aPref, nsAString* aResult)
{
	NS_PRECONDITION(aResult, "aResult must not be NULL");
	std::cout << "[Pref] " << aPref << "\n";
	return NS_OK;
}

// static
nsresult
Preferences::GetDefaultLocalizedCString(const char* aPref,
		nsACString* aResult)
{
	std::cout << "[Pref] " << aPref << "\n";
	return NS_OK;
}

// static
nsresult
Preferences::GetDefaultLocalizedString(const char* aPref,
		nsAString* aResult)
{
	std::cout << "[Pref] " << aPref << "\n";
	return NS_OK;
}

// static
nsAdoptingString
Preferences::GetDefaultString(const char* aPref)
{
	nsAdoptingString result;
	std::cout << "[Pref] " << aPref << "\n";
	return result;
}

// static
nsAdoptingCString
Preferences::GetDefaultCString(const char* aPref)
{
	nsAdoptingCString result;
	std::cout << "[Pref] " << aPref << "\n";
	return result;
}

// static
nsAdoptingString
Preferences::GetDefaultLocalizedString(const char* aPref)
{
	nsAdoptingString result;
	std::cout << "[Pref] " << aPref << "\n";
	return result;
}

// static
nsAdoptingCString
Preferences::GetDefaultLocalizedCString(const char* aPref)
{
	nsAdoptingCString result;
	std::cout << "[Pref] " << aPref << "\n";
	return result;
}

// static
nsresult
Preferences::GetDefaultComplex(const char* aPref, const nsIID &aType,
		void** aResult)
{
	std::cout << "[Pref] " << aPref << "\n";
	return NS_OK;
}

// static
int32_t
Preferences::GetDefaultType(const char* aPref)
{
	NS_ENSURE_TRUE(InitStaticMembers(), nsIPrefBranch::PREF_INVALID);
	std::cout << "[Pref] " << aPref << "\n";
	return 0;
}

} // namespace mozilla
