/*
 * Copyright 2008-2009 Alexander Egorov <egoroff@gmail.com> (http://www.egoroff.spb.ru)
 */

#pragma once

/**
* \brief SQL query wrapper. Decorates SQL queries (if necessary) to cleanup and speed up queries in some cases.
*/
class QueryDecorator {
 public:
	/**
	* \brief Creates new wrapper instance using original query specified
	* @param pOriginalQuery Original query to wrap up
	* @param limit The max number of records to read from DB
	* @param offset The max number of records from the beginning to skip
	* @param isLimit Whether to use limit parameter.
	*/
	QueryDecorator(
		__deref_in LPCWSTR pOriginalQuery,
		unsigned long limit = 0,
		unsigned long offset = 0,
		bool isLimit = false);

	/**
	* \brief Virtual descructor
	*/
	virtual ~QueryDecorator(void);

	/**
	* \brief Gets decorated query
	* \return Decorated query
	*/
	LPCWSTR GetQuery(void) const;

	/**
	* \brief Shows whether is the original query represent SQL SELECT statement
	* \return true if the original query is SELECT statement othewise false
	*/
	__checkReturn bool IsSelectQuery(void) const;

	/**
	* \brief Shows whether is the original query represent SQL EXEC statement
	* \return true if the original query is EXEC statement othewise false
	*/
	__checkReturn bool IsExecQuery(void) const;
 private:
	LPCWSTR pOriginalQuery_;
	bool isSelectQuery_;
	bool isExecQuery_;
	bool isLimit_;
	std::auto_ptr<wchar_t> pDecoratedQuery_;
	DISALLOW_COPY_AND_ASSIGN(QueryDecorator);
};
