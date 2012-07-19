/*!
 * \brief   The file contains QueryDecorator test class implementation
 * \author  \verbatim
            Created by: Alexander Egorov
            \endverbatim
 * \date    \verbatim
            Creation date: 2010-04-11
            \endverbatim
 * Copyright 2008-2012 Alexander Egorov <egoroff@gmail.com> (http://www.egoroff.spb.ru)
 */

#include "StdAfx.h"
#include "..\QueryDecorator.h"
#include "QueryDecoratorTest.h"

const wchar_t* kSelect = L"SELECT * from Table";
const wchar_t* kSelectWithLeadingSpaces = L"  SELECT * from Table";
const wchar_t* kExec = L"EXEC spClient @ClientId = '5C16B5A5-1F80-4B32-A110-9D03A8FC7D9D'";
const wchar_t* kExecWithLeadingSpaces = L"  EXEC spClient @ClientId = '5C16B5A5-1F80-4B32-A110-9D03A8FC7D9D'";
const wchar_t* kDelete = L"DELETE FROM Table WHERE id = 1";

QueryDecoratorTest::QueryDecoratorTest(void)
{}

QueryDecoratorTest::~QueryDecoratorTest(void)
{}

TEST_F(QueryDecoratorTest, SelectWithoutLimit)
{
    QueryDecorator decorator(kSelect);
    EXPECT_STREQ(kSelect, decorator.GetQuery());
}

TEST_F(QueryDecoratorTest, SelectWithoutLimitWithLeadingSpaces)
{
    QueryDecorator decorator(kSelectWithLeadingSpaces);
    EXPECT_STREQ(kSelect, decorator.GetQuery());
}

TEST_F(QueryDecoratorTest, IsSelect)
{
    QueryDecorator decorator(kSelect);
    EXPECT_TRUE(decorator.IsSelectQuery());
    EXPECT_FALSE(decorator.IsExecQuery());
}

TEST_F(QueryDecoratorTest, IsSelectWithLeadingSpaces)
{
    QueryDecorator decorator(kSelectWithLeadingSpaces);
    EXPECT_TRUE(decorator.IsSelectQuery());
    EXPECT_FALSE(decorator.IsExecQuery());
}

TEST_F(QueryDecoratorTest, SelectWithLimit)
{
    QueryDecorator decorator(kSelect, 10, 0, true);
    EXPECT_STREQ(L"SELECT TOP 10 * from Table", decorator.GetQuery());
}

TEST_F(QueryDecoratorTest, SelectWithLimitAndOffset)
{
    QueryDecorator decorator(kSelect, 10, 10, true);
    EXPECT_STREQ(L"SELECT TOP 20 * from Table", decorator.GetQuery());
}

TEST_F(QueryDecoratorTest, SelectWithLimitAndLeadingSpaces)
{
    QueryDecorator decorator(kSelectWithLeadingSpaces, 10, 0, true);
    EXPECT_STREQ(L"SELECT TOP 10 * from Table", decorator.GetQuery());
}

TEST_F(QueryDecoratorTest, SelectWithZeroLimitAndNonZeroOffset)
{
    QueryDecorator decorator(kSelect, 0, 10, true);
    EXPECT_STREQ(L"SELECT TOP 0 * from Table", decorator.GetQuery());
}

TEST_F(QueryDecoratorTest, ExecWithoutLimit)
{
    QueryDecorator decorator(kExec);
    EXPECT_STREQ(kExec, decorator.GetQuery());
}

TEST_F(QueryDecoratorTest, ExecWithoutLimitWithLeadingSpaces)
{
    QueryDecorator decorator(kExecWithLeadingSpaces);
    EXPECT_STREQ(kExec, decorator.GetQuery());
}

TEST_F(QueryDecoratorTest, IsExec)
{
    QueryDecorator decorator(kExec);
    EXPECT_FALSE(decorator.IsSelectQuery());
    EXPECT_TRUE(decorator.IsExecQuery());
}

TEST_F(QueryDecoratorTest, IsExecWithLeadingSpaces)
{
    QueryDecorator decorator(kExecWithLeadingSpaces);
    EXPECT_FALSE(decorator.IsSelectQuery());
    EXPECT_TRUE(decorator.IsExecQuery());
}

TEST_F(QueryDecoratorTest, ExecWithLimitAndOffset)
{
    QueryDecorator decorator(kExec, 10, 10, true);
    EXPECT_STREQ(kExec, decorator.GetQuery());
}

TEST_F(QueryDecoratorTest, Delete)
{
    QueryDecorator decorator(kDelete);
    EXPECT_STREQ(kDelete, decorator.GetQuery());
}

TEST_F(QueryDecoratorTest, IsNotExecAndNotSelect)
{
    QueryDecorator decorator(kDelete);
    EXPECT_FALSE(decorator.IsSelectQuery());
    EXPECT_FALSE(decorator.IsExecQuery());
}

TEST_F(QueryDecoratorTest, DeleteWithLimitAndOffset)
{
    QueryDecorator decorator(kDelete, 10, 10, true);
    EXPECT_STREQ(kDelete, decorator.GetQuery());
}
