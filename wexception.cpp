/*!
 * \brief   The file contains wide char exception implementation
 * \author  \verbatim
            Created by: Alexander Egorov
            \endverbatim
 * \date    \verbatim
            Creation date: 2010-04-11
            \endverbatim
 * Copyright 2008-2012 Alexander Egorov <egoroff@gmail.com> (http://www.egoroff.spb.ru)
 */

#include "StdAfx.h"
#include "wexception.h"

wexception::wexception()
try : pMessage_(new std::wstring)
        , pMessageStream_(NULL) {} catch (const exception&) {
        if (pMessage_ != NULL) {
            delete pMessage_;
        }
    }
;

wexception::wexception(const wexception& rException)
    : pMessageStream_(rException.pMessageStream_)
{
    pMessage_ = new std::wstring(*(rException.pMessage_));
}

wexception::wexception(const std::wstring* pMessage)
    : pMessage_(pMessage)
    , pMessageStream_(NULL)
{}

wexception & wexception::operator=(const wexception& rException)
{
    if (this != &rException) {
        this->wexception::~wexception();
        new (this)wexception(rException);
    }
    return *this;
}

wexception::wexception(const std::wostringstream* pMessageStream)
    : pMessageStream_(pMessageStream)
{
    pMessage_ = new std::wstring(pMessageStream_->str());
}

wexception::wexception(const wchar_t* pMessage) : pMessageStream_(NULL)
{
    assert(pMessage != NULL);
    pMessage_ = new std::wstring(pMessage);
}

wexception::~wexception()
{
    if (pMessage_ != NULL) {
        delete pMessage_;
    } else {
        assert(FALSE);
    }
    if (pMessageStream_ != NULL) {
        delete pMessageStream_;
    }
}

const std::wstring* wexception::GetMessage() const
{
    return pMessage_;
}

