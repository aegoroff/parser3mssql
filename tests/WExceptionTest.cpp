/*
 * Copyright 2008-2009 Alexander Egorov <egoroff@gmail.com> (http://www.egoroff.spb.ru)
 */

#include "StdAfx.h"
#include "..\wexception.h"
#include "WExceptionTest.h"

using namespace std;

const wchar_t* c_pwszMessage = L"An exception occured";

WExceptionTest::WExceptionTest(void) {
}

WExceptionTest::~WExceptionTest(void) {
}

TEST_F(WExceptionTest, DefaultConstructor )
{
    wexception exc;
    EXPECT_EQ(0, exc.GetMessage() -> length());
}

TEST_F(WExceptionTest, StringConstructor )
{
    wstring* pMessage = new wstring(c_pwszMessage);
    wexception exc(pMessage);
    EXPECT_STREQ(c_pwszMessage, exc.GetMessage()->c_str());
}

TEST_F(WExceptionTest, StreamConstructor )
{
    wostringstream* pMessageStream = new wostringstream;
    *pMessageStream << c_pwszMessage;
    wexception exc(pMessageStream);
    EXPECT_STREQ(c_pwszMessage, exc.GetMessage()->c_str());
}

TEST_F(WExceptionTest, CopyConstructor )
{
    wexception exc(c_pwszMessage);
    wexception excCopy(exc);
    EXPECT_STREQ(c_pwszMessage, excCopy.GetMessage()->c_str());
}

TEST_F(WExceptionTest, AssignOperator )
{
    wexception exc(c_pwszMessage);
    wexception excCopy;
    excCopy = exc;
    EXPECT_STREQ(c_pwszMessage, excCopy.GetMessage()->c_str());
}
