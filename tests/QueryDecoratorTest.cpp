/*
 * Copyright 2008-2009 Alexander Egorov <egoroff@gmail.com> (http://www.egoroff.spb.ru)
 */

#include "StdAfx.h"
#include "..\QueryDecorator.h"
#include "QueryDecoratorTest.h"

const wchar_t* c_pszSelect = L"SELECT * from Table";
const wchar_t* c_pszSelectWithLeadingSpaces = L"  SELECT * from Table";
const wchar_t* c_pszExec = L"EXEC spClient @ClientId = '5C16B5A5-1F80-4B32-A110-9D03A8FC7D9D'";
const wchar_t* c_pszExecWithLeadingSpaces = L"  EXEC spClient @ClientId = '5C16B5A5-1F80-4B32-A110-9D03A8FC7D9D'";
const wchar_t* c_pszDelete = L"DELETE FROM Table WHERE id = 1";

QueryDecoratorTest::QueryDecoratorTest(void)
{
}

QueryDecoratorTest::~QueryDecoratorTest(void)
{
}

TEST_F(QueryDecoratorTest, SelectWithoutLimit) {
    QueryDecorator decorator(c_pszSelect);
    EXPECT_STREQ(c_pszSelect, decorator.GetQuery());
}

TEST_F(QueryDecoratorTest, SelectWithoutLimitWithLeadingSpaces) {
    QueryDecorator decorator(c_pszSelectWithLeadingSpaces);
    EXPECT_STREQ(c_pszSelect, decorator.GetQuery());
}

TEST_F(QueryDecoratorTest, IsSelect) {
    QueryDecorator decorator(c_pszSelect);
    EXPECT_TRUE(decorator.IsSelectQuery());
    EXPECT_FALSE(decorator.IsExecQuery());
}

TEST_F(QueryDecoratorTest, IsSelectWithLeadingSpaces) {
    QueryDecorator decorator(c_pszSelectWithLeadingSpaces);
    EXPECT_TRUE(decorator.IsSelectQuery());
    EXPECT_FALSE(decorator.IsExecQuery());
}

TEST_F(QueryDecoratorTest, SelectWithLimit) {
    QueryDecorator decorator(c_pszSelect, 10, 0, true);
    EXPECT_STREQ(L"SELECT TOP 10 * from Table", decorator.GetQuery());
}

TEST_F(QueryDecoratorTest, SelectWithLimitAndOffset) {
    QueryDecorator decorator(c_pszSelect, 10, 10, true);
    EXPECT_STREQ(L"SELECT TOP 20 * from Table", decorator.GetQuery());
}

TEST_F(QueryDecoratorTest, SelectWithLimitAndLeadingSpaces) {
    QueryDecorator decorator(c_pszSelectWithLeadingSpaces, 10, 0, true);
    EXPECT_STREQ(L"SELECT TOP 10 * from Table", decorator.GetQuery());
}

TEST_F(QueryDecoratorTest, SelectWithZeroLimitAndNonZeroOffset) {
    QueryDecorator decorator(c_pszSelect, 0, 10, true);
    EXPECT_STREQ(L"SELECT TOP 0 * from Table", decorator.GetQuery());
}

TEST_F(QueryDecoratorTest, ExecWithoutLimit) {
    QueryDecorator decorator(c_pszExec);
    EXPECT_STREQ(c_pszExec, decorator.GetQuery());
}

TEST_F(QueryDecoratorTest, ExecWithoutLimitWithLeadingSpaces) {
    QueryDecorator decorator(c_pszExecWithLeadingSpaces);
    EXPECT_STREQ(c_pszExec, decorator.GetQuery());
}

TEST_F(QueryDecoratorTest, IsExec) {
    QueryDecorator decorator(c_pszExec);
    EXPECT_FALSE(decorator.IsSelectQuery());
    EXPECT_TRUE(decorator.IsExecQuery());
}

TEST_F(QueryDecoratorTest, IsExecWithLeadingSpaces) {
    QueryDecorator decorator(c_pszExecWithLeadingSpaces);
    EXPECT_FALSE(decorator.IsSelectQuery());
    EXPECT_TRUE(decorator.IsExecQuery());
}

TEST_F(QueryDecoratorTest, ExecWithLimitAndOffset) {
    QueryDecorator decorator(c_pszExec, 10, 10, true);
    EXPECT_STREQ(c_pszExec, decorator.GetQuery());
}

TEST_F(QueryDecoratorTest, Delete) {
    QueryDecorator decorator(c_pszDelete);
    EXPECT_STREQ(c_pszDelete, decorator.GetQuery());
}

TEST_F(QueryDecoratorTest, IsNotExecAndNotSelect) {
    QueryDecorator decorator(c_pszDelete);
    EXPECT_FALSE(decorator.IsSelectQuery());
    EXPECT_FALSE(decorator.IsExecQuery());
}

TEST_F(QueryDecoratorTest, DeleteWithLimitAndOffset) {
    QueryDecorator decorator(c_pszDelete, 10, 10, true);
    EXPECT_STREQ(c_pszDelete, decorator.GetQuery());
}
