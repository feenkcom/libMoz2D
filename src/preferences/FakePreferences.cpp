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
#include <fstream>

namespace mozilla {

static bool DEBUG = true;
#define LOG(X) if (DEBUG) { std::ofstream log; log.open("PreferencesLog.txt", std::ios::out | std::ios::app); log << X << "\n"; log.close(); };


enum {
	eNoType = 0,
	eBooleanType = 1,
	eIntType = 2,
	eFloatType = 3,
	eStringType = 4,
	eISupportsType = 5
};

struct HashEntry : public PLDHashEntryHdr
{
	nsCString mEntryName;
	uint8_t mEntryType;
	union
	{
		int32_t mInt;
		nsCString* mString;
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
		case eBooleanType:
			mData.mInt = aRHS.mData.mInt;
			break;
		case eIntType:
			mData.mInt = aRHS.mData.mInt;
			break;
		case eFloatType:
			NS_ASSERTION(aRHS.mData.mString, "Source entry has no string");
			mData.mString = new nsCString(*aRHS.mData.mString);
			break;
		case eStringType:
			NS_ASSERTION(aRHS.mData.mString, "Source entry has no string");
			mData.mString = new nsCString(*aRHS.mData.mString);
			break;
		default:
			NS_ERROR("Unknown type");
		}
	}

	~HashEntry() {
		Reset(eNoType); }

	void Reset(uint8_t aNewType)
	{
		if (mData.mString)
			delete mData.mString;
		mData.mString = nullptr;
		mData.mInt = 0;
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
		foundEntry->Reset(aEntryType);
		return foundEntry;
	}

	foundEntry = static_cast<HashEntry*>(table->Add((void*)aName, fallible));
	if (!foundEntry) {
		return nullptr;
	}

	// Use placement new. Our actor does not clobber keyHash, which is important.
	new (foundEntry) HashEntry(aEntryType, aName);
	return foundEntry;
}

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

	if (DEBUG) {
		std::ofstream log;
		log.open("PreferencesLog.txt");
		log.close();
	}
	
	table = new PLDHashTable(&sHashOps, sizeof(HashEntry), 8192);
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
	HashEntry* foundEntry = GetNamedEntry(aName);
	if (foundEntry && (foundEntry->mEntryType == eBooleanType || foundEntry->mEntryType == eIntType)) {
		*aRetVal = (foundEntry->mData.mInt);
		LOG("[Get BoolIn] " << aName << ": " << (((bool)foundEntry->mData.mInt) ? "true" : "false"));
		return NS_OK;
	}
	*aRetVal = false;
	LOG("[Get BoolIn] " << aName << ": " << "Not found!");
	return NS_ERROR_FAILURE;
}

// static
nsresult
Preferences::GetInt(const char* aName, int32_t* aRetVal)
{
	HashEntry* foundEntry = GetNamedEntry(aName);
	if (foundEntry && (foundEntry->mEntryType == eBooleanType || foundEntry->mEntryType == eIntType)) {
		*aRetVal = foundEntry->mData.mInt;
		LOG("[Get IntIn] " << aName << ": " << foundEntry->mData.mInt);
		return NS_OK;
	}
	*aRetVal = 0;
	LOG("[Get IntIn] " << aName << ": " << "Not found!");
	return NS_ERROR_FAILURE;
}

// static
nsresult
Preferences::GetFloat(const char* aName, float* aRetVal)
{
	HashEntry* foundEntry = GetNamedEntry(aName);
	// if entry type is encoded as string, try to convert it to float
	if (foundEntry && (foundEntry->mEntryType == eStringType || foundEntry->mEntryType == eFloatType)) {
		nsAdoptingCString result = nsAdoptingCString(ToNewCString(*(foundEntry->mData.mString)));
		nsresult rv = NS_OK;
		LOG("[Get FloatIn] " << aName << ": " << (foundEntry->mData.mString));
		*aRetVal = result.ToFloat(&rv);
		return rv;
	}
	// otherwise convert int
	if (foundEntry && foundEntry->mEntryType == eIntType) {
		nsresult rv = NS_OK;
		LOG("[Get FloatIn] " << aName << ": " << (foundEntry->mData.mInt));
		*aRetVal = foundEntry->mData.mInt;
		return rv;
	}
	*aRetVal = 0.f;
	LOG("[Get FloatIn] " << aName << ": " << "Not found!");
	return NS_ERROR_FAILURE;
}

// static
nsAdoptingCString
Preferences::GetCString(const char* aName)
{
	nsAdoptingCString result;
	HashEntry* foundEntry = GetNamedEntry(aName);
	if (foundEntry && foundEntry->mEntryType == eStringType) {
		LOG("[Get CString] " << aName << ": \"" << ((foundEntry->mData.mString)->get()) << "\"");
		result = nsAdoptingCString(ToNewCString(*(foundEntry->mData.mString)));
		return result;
	}
	LOG("[Get CString] " << aName << ": " << "Not found!");
	return result;
}

// static
nsAdoptingString
Preferences::GetString(const char* aName)
{
	nsAdoptingString result;
	HashEntry* foundEntry = GetNamedEntry(aName);
	if (foundEntry && foundEntry->mEntryType == eStringType) {
		result = NS_ConvertUTF8toUTF16(ToNewCString(*(foundEntry->mData.mString)));
		LOG("[Get String] " << aName << ": \"" << ((foundEntry->mData.mString)->get()) << "\"");
		return result;
	}
	LOG("[Get String] " << aName << ": " << "Not found!");
	return result;
}

// static
nsresult
Preferences::GetCString(const char* aName, nsACString* aRetVal)
{
	nsAdoptingCString result;
	HashEntry* foundEntry = GetNamedEntry(aName);
	if (foundEntry && foundEntry->mEntryType == eStringType) {
		result = nsAdoptingCString(ToNewCString(*(foundEntry->mData.mString)));
		LOG("[Get CStringIn] " << aName << ": \"" << ((foundEntry->mData.mString)->get()) << "\"");
		*aRetVal = result;
		return NS_OK;
	}
	*aRetVal = result;
	LOG("[Get CStringIn] " << aName << ": " << "Not found!");
	return NS_ERROR_FAILURE;
}

// static
nsresult
Preferences::GetString(const char* aName, nsAString* aRetVal)
{
	nsAdoptingString result;
	HashEntry* foundEntry = GetNamedEntry(aName);
	if (foundEntry && foundEntry->mEntryType == eStringType) {
		result = NS_ConvertUTF8toUTF16(ToNewCString(*(foundEntry->mData.mString)));
		LOG("[Get StringIn] " << aName << ": \"" << ((foundEntry->mData.mString)->get()) << "\"");
		*aRetVal = result;
		return NS_OK;
	}
	*aRetVal = result;
	LOG("[Get StringIn] " << aName << ": " << "Not found!");
	return NS_ERROR_FAILURE;
}

// static
nsAdoptingCString
Preferences::GetLocalizedCString(const char* aName)
{
	nsAdoptingCString result;

	LOG("[Get LocalizedCString] " << aName << ": " << "Not implemented!");

	return result;
}

// static
nsAdoptingString
Preferences::GetLocalizedString(const char* aName)
{
	nsAdoptingString result;

	LOG("[Get LocalizedString] " << aName << ": " << "Not implemented!");

	return result;
}

// static
nsresult
Preferences::GetLocalizedCString(const char* aName, nsACString* aResult)
{
	LOG("[Get LocalizedCStringIn] " << aName << ": " << "Not implemented!");
	return NS_ERROR_FAILURE;
}

// static
nsresult
Preferences::GetLocalizedString(const char* aName, nsAString* aResult)
{

	LOG("[Get LocalizedStringIn] " << aName << ": " << "Not implemented!");
	return NS_ERROR_FAILURE;
}

// static
nsresult
Preferences::GetComplex(const char* aName, const nsIID &aType, void** aResult)
{
	LOG("[Get ComplexIn] " << aName << ": " << "Not implemented!");
	return NS_ERROR_FAILURE;
}

// static
nsresult
Preferences::SetCString(const char* aPref, const char* aValue)
{
	HashEntry* foundEntry = GetOrMakeEntry(aPref, eStringType);
	if (!foundEntry) {
		return NS_ERROR_OUT_OF_MEMORY;
	}
	LOG("[Set CString] " << aPref << " : \"" << (aValue) << "\"");
	foundEntry->mData.mString = new nsCString(aValue);
	return NS_OK;
}

// static
nsresult
Preferences::SetCString(const char* aPref, const nsACString &aValue)
{
	HashEntry* foundEntry = GetOrMakeEntry(aPref, eStringType);
	if (!foundEntry) {
		return NS_ERROR_OUT_OF_MEMORY;
	}
	foundEntry->mData.mString = new nsCString(ToNewCString(aValue));
	return NS_OK;
}

// static
nsresult
Preferences::SetString(const char* aPref, const char16ptr_t aValue)
{
	HashEntry* foundEntry = GetOrMakeEntry(aPref, eStringType);
	if (!foundEntry) {
		return NS_ERROR_OUT_OF_MEMORY;
	}
	foundEntry->mData.mString = new nsCString(NS_ConvertUTF16toUTF8(aValue));
	return NS_OK;
}

// static
nsresult
Preferences::SetString(const char* aPref, const nsAString &aValue)
{
	HashEntry* foundEntry = GetOrMakeEntry(aPref, eStringType);
	if (!foundEntry) {
		return NS_ERROR_OUT_OF_MEMORY;
	}
	foundEntry->mData.mString = new nsCString(NS_ConvertUTF16toUTF8(aValue));
	return NS_OK;
}

// static
nsresult
Preferences::SetBool(const char* aPref, bool aValue)
{
	HashEntry* foundEntry = GetOrMakeEntry(aPref, eBooleanType);
	if (!foundEntry) {
		return NS_ERROR_OUT_OF_MEMORY;
	}
	LOG("[Set Bool] " << aPref << " : " << (aValue ? "true" : "false"));
	foundEntry->mData.mInt = aValue;
	return NS_OK;
}

// static
nsresult
Preferences::SetInt(const char* aPref, int32_t aValue)
{
	HashEntry* foundEntry = GetOrMakeEntry(aPref, eIntType);
	if (!foundEntry) {
		return NS_ERROR_OUT_OF_MEMORY;
	}
	LOG("[Set Int] " << aPref << " : " << aValue);
	foundEntry->mData.mInt = aValue;
	return NS_OK;
}

// static
nsresult
Preferences::SetFloat(const char* aPref, float aValue)
{
	HashEntry* foundEntry = GetOrMakeEntry(aPref, eIntType);
	if (!foundEntry) {
		return NS_ERROR_OUT_OF_MEMORY;
	}
	LOG("[Set Float] " << aPref << " : " << aValue);
	foundEntry->mData.mString = new nsCString(nsPrintfCString("%f", aValue).get());
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
	HashEntry* foundEntry = GetNamedEntry(aPref);
	if (!foundEntry) {
		return PREF_INVALID;
	}
	int32_t type = PREF_INVALID;
	switch (foundEntry->mEntryType) {
	case eBooleanType:
		type = PREF_BOOL;
		break;
	case eIntType:
		type = PREF_INT;
		break;
	case eFloatType:
		type = PREF_STRING;
		break;
	case eStringType:
		type = PREF_STRING;
		break;
	case eNoType:
	default:
		type = PREF_INVALID;
		break;
	}
	return type;
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
