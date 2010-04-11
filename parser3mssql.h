/** @file
	Parser MSSQL driver.
	Copyright 2008-2009 Alexander Egorov <egoroff@gmail.com> (http://www.egoroff.spb.ru)
*/

#ifndef _MSC_VER
#	error compile ISAPI module with MSVC [no urge for now to make it autoconf-ed (PAF)]
#endif

#include "stdafx.h"
#include "pa_sql_driver.h"
#include "Client.h"
#include "QueryDecorator.h"

struct Connection {
	SQL_Driver_services* services;
	Client* pClient;
};

const DBORDINAL maxColumnsCount = 500;

struct ColumnBind {
	ColumnBind() {
		memset(this, 0, sizeof(this));
	}
	LPWSTR szValue;
	DBLENGTH nSize;
	DWORD dwStatus;
};

/**
* \brief MS SQL server native driver
*/
class SqlNativeDriver : public SQL_Driver {
 public:

	/**
	* \brief Creates driver instance
	*/
	SqlNativeDriver();

	/**
	* \brief gets api version
	*/
	int api_version();

	/**
	* \brief SQL_Driver initialize implementation just returns zero and does nothing
	*/
	const char* initialize(char* p);

	/**
	* \brief creates SQL server connection
	* @param pConnString the first string to compare
	*		format: @b server=<server>;database=<database>;Trusted_Connection=Yes (MSSQL connect string)
	*			WARNING: must be used only to connect, for buffer doesn't live long
	* @param rServices SQL Driver helpers
	* @param pConnection Connection object
	*/
	void connect(
		__in char* pConnString,
		SQL_Driver_services& rServices,
		__deref_out void** pConnection);

	/**
	* \brief SQL_Driver initialize implementation just returns zero and does nothing
	* @param pConnection Connection object that is destroyed in the method
	*/
	void disconnect(__in void* pConnection);

	/**
	* \brief Commits a transaction. Does nothing
	*/
	void commit(void* p);

	/**
	* \brief Rollbacks a transaction. Does nothing
	*/
	void rollback(void* p);

	/**
	* \brief Checks whether a connection is alive. Always returns true
	*/
	bool ping(void* p);

	/**
	* \brief Does special symbols escaping
	* @param pConnection Connection object
	* @param from Pointer to SQL query to escape
	* @param length SQL query buffer length
	*/
	const char* quote(
		__in void* pConnection,
		__in const char* from,
		unsigned int length);

	/**
	* \brief Runs SQL query
	* @param pConnection Connection object
	* @param nPlaceholders The number of bind variables. Not used so far
	* @param pPlaceholders The pointer to bind variables data. Not used so far
	* @param offset The number of records to offset
	* @param limit The max number of records to return
	* @param rHandlers Query results handler
	*/
	void query(__in void* pConnection,
		__in const char* pStatement,
		size_t nPlaceholders,
		__in Placeholder* pPlaceholders,
		unsigned long offset,
		unsigned long limit,
		SQL_Driver_query_event_handlers& rHandlers);

 private:
	void _throw(SQL_Driver_services&, const std::exception&) const;
	void ThrowIfError(SQL_Driver_services&, bool isFailure) const;
	void ValidateColumns(
		ULONG* pColumnsCount, SQL_Driver_services& rServices) const;

	void InsertColumn(
		const CStringW& rUnicodeStr,
		Connection& rConnection,
		SQL_Driver_query_event_handlers& rHandlers,
		SQL_Error& rSqlError
	);

	void InsertColumnValue(
		const CStringA& rMultibyteStr,
		Connection& rConnection,
		SQL_Driver_query_event_handlers& rHandlers,
		SQL_Error& rSqlError
	);

	void ExecuteReader(
		const QueryDecorator& decorator,
		Connection& rConnection,
		SQL_Driver_query_event_handlers& rHandlers,
		unsigned long offset,
		unsigned long limit);

	void ExecuteReader(
		const QueryDecorator& decorator,
		Connection& rConnection,
		SQL_Driver_query_event_handlers& rHandlers,
		unsigned long offset,
		unsigned long limit,
		size_t nPlaceholders,
		__in Placeholder* pPlaceholders);

	template<class TColumnSource, typename TAccessor>
	void ReadResults(
		CCommand<TAccessor>& cmd,
		Connection& rConnection,
		SQL_Driver_query_event_handlers& rHandlers,
		unsigned long offset,
		unsigned long limit,
		ULONG nColumns,
		TColumnSource src);

	template<class TColumnSource>
	LPWSTR ReadValue(int i, TColumnSource src);

	UINT codePage_;
};

template<>
LPWSTR SqlNativeDriver::ReadValue(int i, ColumnBind* pBind) {
	return pBind[i-1].szValue;
}

template<>
LPWSTR SqlNativeDriver::ReadValue(
								  int i,
								  CCommand<CDynamicStringAccessorW>* pCmd) {
	return pCmd ->GetString(i);
}

template<class TColumnSource, typename TAccessor>
void SqlNativeDriver::ReadResults(
	CCommand<TAccessor>& cmd,
	Connection& rConnection,
	SQL_Driver_query_event_handlers& rHandlers,
	unsigned long offset,
	unsigned long limit,
	ULONG nColumns,
	TColumnSource src) {
	SQL_Error sqlError;
	HRESULT result = NULL;
	SQL_Driver_services& rServices = *rConnection.services;

	ThrowIfError(rServices, rHandlers.before_rows(sqlError));
	if(offset > 0) {
		result = cmd.MoveNext(offset - 1);
	}

	unsigned long row = 0;
	while(SUCCEEDED(result) && result != DB_S_ENDOFROWSET && (IsNoLimit(limit) || row < limit)) {
		ThrowIfError(rServices, rHandlers.add_row(sqlError));
		for(DBORDINAL i = 1; i <= nColumns; ++i) {
			const CStringA mbStr = CW2A(ReadValue(i, src), codePage_);
			InsertColumnValue(mbStr, rConnection, rHandlers, sqlError);
		}
		result = cmd.MoveNext();
		++row;
	}
}

extern "C" SQL_Driver *SQL_DRIVER_CREATE() {
	return new SqlNativeDriver();
}
