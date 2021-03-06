/*!
 * \brief   The file contains SQL queries decorator implementation
 * \author  \verbatim
            Created by: Alexander Egorov
            \endverbatim
 * \date    \verbatim
            Creation date: 2010-04-11
            \endverbatim
 * Copyright 2008-2012 Alexander Egorov <egoroff@gmail.com> (http://www.egoroff.spb.ru)
 */

#include "StdAfx.h"
#include "QueryDecorator.h"

static const int kSelectLength = 6;
static const int kExecLength = 4;
static const int kSelectTopLength = 11;
static const int kMaxTopLength = 16;

/**
 * \brief Compares two strings and retuns true if they are equal false otherwise.
 * @param lpString1 the first string to compare
 * @param lpString2 the second string to compare
 * @param cchCount the second string length
 */
inline bool AreStrEqual(
    __in LPCWSTR lpString1,
    __in LPCWSTR lpString2,
    int          cchCount)
{
    return ::CompareStringW(
               LOCALE_INVARIANT,
               NORM_IGNORECASE,
               lpString1,
               cchCount,
               lpString2,
               cchCount) == CSTR_EQUAL;
}

QueryDecorator::QueryDecorator(
    LPCWSTR       pOriginalQuery,
    unsigned long limit,
    unsigned long offset,
    bool          isLimit)
    :    pOriginalQuery_(pOriginalQuery)
    , isLimit_(isLimit)
{
    while (iswspace(static_cast<WCHAR>(*pOriginalQuery_))) {
        ++pOriginalQuery_;
    }
    isSelectQuery_ = AreStrEqual(pOriginalQuery_, L"select", kSelectLength);
    isExecQuery_ = AreStrEqual(pOriginalQuery_, L"EXEC", kExecLength);
    if (!isLimit || !isSelectQuery_) {
        return;
    }
    if (limit == 0) {
        offset = 0;
    }
    pOriginalQuery_ += kSelectLength;
    // trailing zero included
    size_t nBuffer =
        kSelectTopLength + kMaxTopLength + wcslen(pOriginalQuery_) + 1;
    pDecoratedQuery_ = std::auto_ptr<wchar_t>(new wchar_t[nBuffer]);
    swprintf_s(
        pDecoratedQuery_.get(),
        nBuffer - 1 /* without trailing zero */,
        L"SELECT TOP %lu%s",
        limit + offset,
        pOriginalQuery_);
}

QueryDecorator::~QueryDecorator(void)
{}

LPCWSTR QueryDecorator::GetQuery(void) const
{
    return isLimit_ && isSelectQuery_ ? pDecoratedQuery_.get() : pOriginalQuery_;
}

bool QueryDecorator::IsSelectQuery(void) const
{
    return isSelectQuery_;
}

bool QueryDecorator::IsExecQuery(void) const
{
    return isExecQuery_;
}
