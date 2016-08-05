/*
 * Extracted from:
 * dom/base/nsContentUtils.cpp
 *
 *  Created on: Aug 1, 2016
 */

#include "stub.h"

#include "nsContentUtils.h"
#include "nsIContent.h"

template<typename StringType, typename CharType>
void
_ASCIIToLowerInSitu(StringType& aStr)
{
  CharType* iter = aStr.BeginWriting();
  CharType* end = aStr.EndWriting();
  MOZ_ASSERT(iter && end);

  while (iter != end) {
    CharType c = *iter;
    if (c >= 'A' && c <= 'Z') {
      *iter = c + ('a' - 'A');
    }
    ++iter;
  }
}

/* static */
void
nsContentUtils::ASCIIToLower(nsAString& aStr)
{
  return _ASCIIToLowerInSitu<nsAString, char16_t>(aStr);
}

/* static */
void
nsContentUtils::ASCIIToLower(nsACString& aStr)
{
  return _ASCIIToLowerInSitu<nsACString, char>(aStr);
}

template<typename StringType, typename CharType>
void
_ASCIIToLowerCopy(const StringType& aSource, StringType& aDest)
{
  uint32_t len = aSource.Length();
  aDest.SetLength(len);
  MOZ_ASSERT(aDest.Length() == len);

  CharType* dest = aDest.BeginWriting();
  MOZ_ASSERT(dest);

  const CharType* iter = aSource.BeginReading();
  const CharType* end = aSource.EndReading();
  while (iter != end) {
    CharType c = *iter;
    *dest = (c >= 'A' && c <= 'Z') ?
       c + ('a' - 'A') : c;
    ++iter;
    ++dest;
  }
}

/* static */
void
nsContentUtils::ASCIIToLower(const nsAString& aSource, nsAString& aDest) {
  return _ASCIIToLowerCopy<nsAString, char16_t>(aSource, aDest);
}

/* static */
void
nsContentUtils::ASCIIToLower(const nsACString& aSource, nsACString& aDest) {
  return _ASCIIToLowerCopy<nsACString, char>(aSource, aDest);
}

bool nsIContent::GetAttr(int32_t aNameSpaceID, nsIAtom* aName,
             nsAString& aResult) const {
	STUB("nsIContent::GetAttr");
}
