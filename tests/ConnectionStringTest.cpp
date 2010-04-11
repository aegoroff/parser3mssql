/*
 * Copyright 2008-2009 Alexander Egorov <egoroff@gmail.com> (http://www.egoroff.spb.ru)
 */

#include "StdAfx.h"
#include "..\ConnectionString.h"
#include "ConnectionStringTest.h"

const char* c_pszStringWithCharset = "mssql://server=egoroff-home^;database=Busy^;Trusted_Connection=Yes?ClientCharset=windows-1251";
const char* c_pszStringWithCharsetLowerCase = "mssql://server=egoroff-home^;database=Busy^;Trusted_Connection=Yes?clientcharset=windows-1251";
const char* c_pszStringWithoutCharset = "mssql://server=egoroff-home^;database=Busy^;Trusted_Connection=Yes";
const std::string c_oleString(c_pszStringWithoutCharset);

ConnectionStringTest::ConnectionStringTest(void) {	
}

ConnectionStringTest::~ConnectionStringTest(void) {
}

TEST_F(ConnectionStringTest, Constructor) {
  ConnectionString str(c_pszStringWithCharset);
}

TEST_F(ConnectionStringTest, ParseWithCharset) {
	ConnectionString str(c_pszStringWithCharset);
	str.Parse();
	EXPECT_TRUE(str.ContainsCharset());
	std::string charset("WINDOWS-1251");
	EXPECT_EQ(charset, str.GetCharset());
	EXPECT_EQ(c_oleString, str.GetOleDBString());
}

TEST_F( ConnectionStringTest, ParseWithCharsetLowerCase ) {
	ConnectionString str(c_pszStringWithCharsetLowerCase);
	str.Parse();
	EXPECT_TRUE(str.ContainsCharset());
	std::string charset("WINDOWS-1251");
	EXPECT_EQ(charset, str.GetCharset());
}

TEST_F( ConnectionStringTest, ParseWithoutCharset ) {
	ConnectionString str(c_pszStringWithoutCharset);
	str.Parse();
	EXPECT_FALSE(str.ContainsCharset());
	EXPECT_EQ(c_oleString, str.GetOleDBString());
}

TEST_F( ConnectionStringTest, CopyCharset ) {
	ConnectionString str(c_pszStringWithCharset);
	str.Parse();
	EXPECT_TRUE(str.ContainsCharset());
	std::string charset("WINDOWS-1251");
	EXPECT_EQ(charset, str.GetCharset());
	char* pCharset = new char[str.CharsetLength()];
	str.CopyCharset(pCharset, str.CharsetLength());
	EXPECT_STREQ("WINDOWS-1251", pCharset);
	delete [] pCharset;
}