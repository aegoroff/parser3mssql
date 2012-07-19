/*!
 * \brief   The file contains connection string wrapper implementation
 * \author  \verbatim
            Created by: Alexander Egorov
            \endverbatim
 * \date    \verbatim
            Creation date: 2010-04-11
            \endverbatim
 * Copyright 2008-2012 Alexander Egorov <egoroff@gmail.com> (http://www.egoroff.spb.ru)
 */

#include "StdAfx.h"
#include <algorithm>
#include "ConnectionString.h"

const char* kCharsetPattern = "CLIENTCHARSET=";

ConnectionString::ConnectionString(const char* pRawString)
    : cnString_(pRawString)
    , containsCharset_(false)
{}

ConnectionString::~ConnectionString(void)
{}

void ConnectionString::Parse()
{
    std::string::size_type ixQ = cnString_.find("?");
    if (ixQ == std::string::npos) {
        oleDBString_ = cnString_;
        return;
    }
    oleDBString_ = cnString_.substr(0, ixQ);
    std::transform(cnString_.begin(), cnString_.end(), cnString_.begin(), toupper);
    std::string::size_type ix = cnString_.find(kCharsetPattern);
    containsCharset_ = ix != std::string::npos;
    if (containsCharset_) {
        charset_ = cnString_.substr(ix + strlen(kCharsetPattern));
    }
}

bool ConnectionString::ContainsCharset() const
{
    return containsCharset_;
}

const std::string& ConnectionString::GetCharset() const
{
    return charset_;
}

void ConnectionString::CopyCharset(
    char*                  pDestination,
    std::string::size_type destLength) const
{
    charset_._Copy_s(pDestination, destLength, charset_.length());
    pDestination[charset_.length()] = 0;
}

size_t ConnectionString::CharsetLength() const
{
    return charset_.length() + 1;
}

const std::string& ConnectionString::GetOleDBString() const
{
    return oleDBString_;
}
