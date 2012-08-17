/*!
 * \brief   The file contains wide char exception test class implementation
 * \author  \verbatim
            Created by: Alexander Egorov
            \endverbatim
 * \date    \verbatim
            Creation date: 2010-04-11
            \endverbatim
 * Copyright 2008-2012 Alexander Egorov <egoroff@gmail.com> (http://www.egoroff.spb.ru)
 */

#include "StdAfx.h"
#include "..\wexception.h"
#include "WExceptionTest.h"

using namespace std;

const wchar_t* kMessage = L"An exception occured";

WExceptionTest::WExceptionTest(void)
{}

WExceptionTest::~WExceptionTest(void)
{}

TEST_F(WExceptionTest, DefaultConstructor)
{
    wexception exc;
    EXPECT_EQ(0, exc.GetMessage()->length());
}

TEST_F(WExceptionTest, StringConstructor)
{
    wstring* pMessage = new wstring(kMessage);
    wexception exc(pMessage);
    EXPECT_STREQ(kMessage, exc.GetMessage()->c_str());
}

TEST_F(WExceptionTest, StreamConstructor)
{
    wostringstream* pMessageStream = new wostringstream;
    *pMessageStream << kMessage;
    wexception exc(pMessageStream);
    EXPECT_STREQ(kMessage, exc.GetMessage()->c_str());
}

TEST_F(WExceptionTest, CopyConstructor)
{
    wexception exc(kMessage);
    wexception excCopy(exc);
    EXPECT_STREQ(kMessage, excCopy.GetMessage()->c_str());
}

TEST_F(WExceptionTest, AssignOperator)
{
    wexception exc(kMessage);
    wexception excCopy;
    excCopy = exc;
    EXPECT_STREQ(kMessage, excCopy.GetMessage()->c_str());
}
