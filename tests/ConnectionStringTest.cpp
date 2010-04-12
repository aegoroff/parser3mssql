/*!
 * \brief   The file contains ConnectionString test class implementation
 * \author  \verbatim
            Created by: Alexander Egorov
            \endverbatim
 * \date    \verbatim
            Creation date: 2010-04-11
            \endverbatim
 * Copyright 2008-2009 Alexander Egorov <egoroff@gmail.com> (http://www.egoroff.spb.ru)
*/

#include "StdAfx.h"
#include "..\ConnectionString.h"
#include "ConnectionStringTest.h"

const char* kStringWithCharset = "mssql://server=egoroff-home^;database=Busy^;Trusted_Connection=Yes?ClientCharset=windows-1251";
const char* kStringWithCharsetLowerCase = "mssql://server=egoroff-home^;database=Busy^;Trusted_Connection=Yes?clientcharset=windows-1251";
const char* kStringWithoutCharset = "mssql://server=egoroff-home^;database=Busy^;Trusted_Connection=Yes";
const std::string kOleString(kStringWithoutCharset);

ConnectionStringTest::ConnectionStringTest(void)
{    
}

ConnectionStringTest::~ConnectionStringTest(void)
{
}

TEST_F(ConnectionStringTest, Constructor)
{
	ConnectionString str(kStringWithCharset);
}

TEST_F(ConnectionStringTest, ParseWithCharset)
{
    ConnectionString str(kStringWithCharset);
    str.Parse();
    EXPECT_TRUE(str.ContainsCharset());
    std::string charset("WINDOWS-1251");
    EXPECT_EQ(charset, str.GetCharset());
    EXPECT_EQ(kOleString, str.GetOleDBString());
}

TEST_F(ConnectionStringTest, ParseWithCharsetLowerCase)
{
    ConnectionString str(kStringWithCharsetLowerCase);
    str.Parse();
    EXPECT_TRUE(str.ContainsCharset());
    std::string charset("WINDOWS-1251");
    EXPECT_EQ(charset, str.GetCharset());
}

TEST_F(ConnectionStringTest, ParseWithoutCharset)
{
    ConnectionString str(kStringWithoutCharset);
    str.Parse();
    EXPECT_FALSE(str.ContainsCharset());
    EXPECT_EQ(kOleString, str.GetOleDBString());
}

TEST_F(ConnectionStringTest, CopyCharset)
{
    ConnectionString str(kStringWithCharset);
    str.Parse();
    EXPECT_TRUE(str.ContainsCharset());
    std::string charset("WINDOWS-1251");
    EXPECT_EQ(charset, str.GetCharset());
    char* pCharset = new char[str.CharsetLength()];
    str.CopyCharset(pCharset, str.CharsetLength());
    EXPECT_STREQ("WINDOWS-1251", pCharset);
    delete [] pCharset;
}
