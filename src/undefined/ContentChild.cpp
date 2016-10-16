/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/ContentChild.h"

//-----------------------------------------------------------------------------
// Method definitions for the IPDL type |union PrefValue|
//
namespace mozilla {
namespace dom {
auto PrefValue::MaybeDestroy(Type aNewType) -> bool
{
    if ((mType) == (T__None)) {
        return true;
    }
    if ((mType) == (aNewType)) {
        return false;
    }
    switch (mType) {
    case TnsCString:
        {
            (ptr_nsCString())->~nsCString__tdef();
            break;
        }
    case Tint32_t:
        {
            (ptr_int32_t())->~int32_t__tdef();
            break;
        }
    case Tbool:
        {
            (ptr_bool())->~bool__tdef();
            break;
        }
    default:
        {
            mozilla::ipc::LogicError("not reached");
            break;
        }
    }
    return true;
}

MOZ_IMPLICIT PrefValue::PrefValue(const nsCString& aOther)
{
    new (ptr_nsCString()) nsCString(aOther);
    mType = TnsCString;
}

MOZ_IMPLICIT PrefValue::PrefValue(const int32_t& aOther)
{
    new (ptr_int32_t()) int32_t(aOther);
    mType = Tint32_t;
}

MOZ_IMPLICIT PrefValue::PrefValue(const bool& aOther)
{
    new (ptr_bool()) bool(aOther);
    mType = Tbool;
}

MOZ_IMPLICIT PrefValue::PrefValue(const PrefValue& aOther)
{
    (aOther).AssertSanity();
    switch ((aOther).type()) {
    case TnsCString:
        {
            new (ptr_nsCString()) nsCString((aOther).get_nsCString());
            break;
        }
    case Tint32_t:
        {
            new (ptr_int32_t()) int32_t((aOther).get_int32_t());
            break;
        }
    case Tbool:
        {
            new (ptr_bool()) bool((aOther).get_bool());
            break;
        }
    case T__None:
        {
            break;
        }
    default:
        {
            mozilla::ipc::LogicError("unreached");
            return;
        }
    }
    mType = (aOther).type();
}

PrefValue::~PrefValue()
{
    MaybeDestroy(T__None);
}

auto PrefValue::operator=(const nsCString& aRhs) -> PrefValue&
{
    if (MaybeDestroy(TnsCString)) {
        new (ptr_nsCString()) nsCString;
    }
    (*(ptr_nsCString())) = aRhs;
    mType = TnsCString;
    return (*(this));
}

auto PrefValue::operator=(const int32_t& aRhs) -> PrefValue&
{
    if (MaybeDestroy(Tint32_t)) {
        new (ptr_int32_t()) int32_t;
    }
    (*(ptr_int32_t())) = aRhs;
    mType = Tint32_t;
    return (*(this));
}

auto PrefValue::operator=(const bool& aRhs) -> PrefValue&
{
    if (MaybeDestroy(Tbool)) {
        new (ptr_bool()) bool;
    }
    (*(ptr_bool())) = aRhs;
    mType = Tbool;
    return (*(this));
}

auto PrefValue::operator=(const PrefValue& aRhs) -> PrefValue&
{
    (aRhs).AssertSanity();
    Type t = (aRhs).type();
    switch (t) {
    case TnsCString:
        {
            if (MaybeDestroy(t)) {
                new (ptr_nsCString()) nsCString;
            }
            (*(ptr_nsCString())) = (aRhs).get_nsCString();
            break;
        }
    case Tint32_t:
        {
            if (MaybeDestroy(t)) {
                new (ptr_int32_t()) int32_t;
            }
            (*(ptr_int32_t())) = (aRhs).get_int32_t();
            break;
        }
    case Tbool:
        {
            if (MaybeDestroy(t)) {
                new (ptr_bool()) bool;
            }
            (*(ptr_bool())) = (aRhs).get_bool();
            break;
        }
    case T__None:
        {
            MaybeDestroy(t);
            break;
        }
    default:
        {
            mozilla::ipc::LogicError("unreached");
            break;
        }
    }
    mType = t;
    return (*(this));
}

auto PrefValue::operator==(const nsCString& aRhs) const -> bool
{
    return (get_nsCString()) == (aRhs);
}

auto PrefValue::operator==(const int32_t& aRhs) const -> bool
{
    return (get_int32_t()) == (aRhs);
}

auto PrefValue::operator==(const bool& aRhs) const -> bool
{
    return (get_bool()) == (aRhs);
}

auto PrefValue::operator==(const PrefValue& aRhs) const -> bool
{
    if ((type()) != ((aRhs).type())) {
        return false;
    }

    switch (type()) {
    case TnsCString:
        {
            return (get_nsCString()) == ((aRhs).get_nsCString());
        }
    case Tint32_t:
        {
            return (get_int32_t()) == ((aRhs).get_int32_t());
        }
    case Tbool:
        {
            return (get_bool()) == ((aRhs).get_bool());
        }
    default:
        {
            mozilla::ipc::LogicError("unreached");
            return false;
        }
    }
}

} // namespace dom
} // namespace mozilla

//-----------------------------------------------------------------------------
// Method definitions for the IPDL type |union MaybePrefValue|
//
namespace mozilla {
namespace dom {
auto MaybePrefValue::MaybeDestroy(Type aNewType) -> bool
{
    if ((mType) == (T__None)) {
        return true;
    }
    if ((mType) == (aNewType)) {
        return false;
    }
    switch (mType) {
    case TPrefValue:
        {
            (ptr_PrefValue())->~PrefValue__tdef();
            break;
        }
    case Tnull_t:
        {
            (ptr_null_t())->~null_t__tdef();
            break;
        }
    default:
        {
            mozilla::ipc::LogicError("not reached");
            break;
        }
    }
    return true;
}

MOZ_IMPLICIT MaybePrefValue::MaybePrefValue(const PrefValue& aOther)
{
    new (ptr_PrefValue()) PrefValue(aOther);
    mType = TPrefValue;
}

MOZ_IMPLICIT MaybePrefValue::MaybePrefValue(const null_t& aOther)
{
    new (ptr_null_t()) null_t(aOther);
    mType = Tnull_t;
}

MOZ_IMPLICIT MaybePrefValue::MaybePrefValue(const MaybePrefValue& aOther)
{
    (aOther).AssertSanity();
    switch ((aOther).type()) {
    case TPrefValue:
        {
            new (ptr_PrefValue()) PrefValue((aOther).get_PrefValue());
            break;
        }
    case Tnull_t:
        {
            new (ptr_null_t()) null_t((aOther).get_null_t());
            break;
        }
    case T__None:
        {
            break;
        }
    default:
        {
            mozilla::ipc::LogicError("unreached");
            return;
        }
    }
    mType = (aOther).type();
}

MaybePrefValue::~MaybePrefValue()
{
    MaybeDestroy(T__None);
}

auto MaybePrefValue::operator=(const PrefValue& aRhs) -> MaybePrefValue&
{
    if (MaybeDestroy(TPrefValue)) {
        new (ptr_PrefValue()) PrefValue;
    }
    (*(ptr_PrefValue())) = aRhs;
    mType = TPrefValue;
    return (*(this));
}

auto MaybePrefValue::operator=(const null_t& aRhs) -> MaybePrefValue&
{
    if (MaybeDestroy(Tnull_t)) {
        new (ptr_null_t()) null_t;
    }
    (*(ptr_null_t())) = aRhs;
    mType = Tnull_t;
    return (*(this));
}

auto MaybePrefValue::operator=(const MaybePrefValue& aRhs) -> MaybePrefValue&
{
    (aRhs).AssertSanity();
    Type t = (aRhs).type();
    switch (t) {
    case TPrefValue:
        {
            if (MaybeDestroy(t)) {
                new (ptr_PrefValue()) PrefValue;
            }
            (*(ptr_PrefValue())) = (aRhs).get_PrefValue();
            break;
        }
    case Tnull_t:
        {
            if (MaybeDestroy(t)) {
                new (ptr_null_t()) null_t;
            }
            (*(ptr_null_t())) = (aRhs).get_null_t();
            break;
        }
    case T__None:
        {
            MaybeDestroy(t);
            break;
        }
    default:
        {
            mozilla::ipc::LogicError("unreached");
            break;
        }
    }
    mType = t;
    return (*(this));
}

auto MaybePrefValue::operator==(const PrefValue& aRhs) const -> bool
{
    return (get_PrefValue()) == (aRhs);
}

auto MaybePrefValue::operator==(const null_t& aRhs) const -> bool
{
    return (get_null_t()) == (aRhs);
}

auto MaybePrefValue::operator==(const MaybePrefValue& aRhs) const -> bool
{
    if ((type()) != ((aRhs).type())) {
        return false;
    }

    switch (type()) {
    case TPrefValue:
        {
            return (get_PrefValue()) == ((aRhs).get_PrefValue());
        }
    case Tnull_t:
        {
            return (get_null_t()) == ((aRhs).get_null_t());
        }
    default:
        {
            mozilla::ipc::LogicError("unreached");
            return false;
        }
    }
}

} // namespace dom
} // namespace mozilla
//-----------------------------------------------------------------------------
// Method definitions for the IPDL type |struct PrefSetting|
//
namespace mozilla {
namespace dom {
MOZ_IMPLICIT PrefSetting::PrefSetting() :
    name_()
{
    Init();
}

PrefSetting::~PrefSetting()
{
}

auto PrefSetting::operator==(const PrefSetting& _o) const -> bool
{
    if ((!((name()) == ((_o).name())))) {
        return false;
    }
    if ((!((defaultValue()) == ((_o).defaultValue())))) {
        return false;
    }
    if ((!((userValue()) == ((_o).userValue())))) {
        return false;
    }
    return true;
}

auto PrefSetting::Init() -> void
{
}

auto PrefSetting::Assign(
        const nsCString& _name,
        const MaybePrefValue& _defaultValue,
        const MaybePrefValue& _userValue) -> void
{
    name_ = _name;
    defaultValue_ = _defaultValue;
    userValue_ = _userValue;
}

} // namespace dom
} // namespace mozilla
