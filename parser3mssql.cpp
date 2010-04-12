/*!
 * \brief   The file contains  Parser MSSQL driver implementation
 * \author  \verbatim
            Created by: Alexander Egorov
            \endverbatim
 * \date    \verbatim
            Creation date: 2010-04-11
            \endverbatim
 * Copyright 2008-2009 Alexander Egorov <egoroff@gmail.com> (http://www.egoroff.spb.ru)
*/

#include "StdAfx.h"
#include "parser3mssql.h"
#include "ConnectionString.h"
#include "wexception.h"

inline static bool IsNoLimit(unsigned long limit)
{
    return limit == SQL_NO_LIMIT;
}

inline void SqlNativeDriver::_throw(
                                    SQL_Driver_services& rServices,
                                    const std::exception& e) const
{
    rServices._throw(e.what());
}

inline void SqlNativeDriver::ThrowIfError(
    SQL_Driver_services& rServices,
    bool isFailure) const
{
    SQL_Error sqlError;
    if(isFailure) {
        rServices._throw(sqlError);
    }
}

inline void SqlNativeDriver::ValidateColumns(ULONG* pColumnsCount, SQL_Driver_services& rServices) const
{
    if (pColumnsCount == NULL) {
        rServices._throw("null column number pointer");
    }
    if(!(*pColumnsCount)) {
        rServices._throw("result contains no columns");
    }
    if(*pColumnsCount > kMaxColumnsCount) {
        *pColumnsCount = kMaxColumnsCount;
    }
}

SqlNativeDriver::SqlNativeDriver() : SQL_Driver() {}

int SqlNativeDriver::api_version()
{
    return SQL_DRIVER_API_VERSION;
}

const char* SqlNativeDriver::initialize(char* p)
{
    UNREFERENCED_PARAMETER(p);
    return 0;
}

/**    connect
    @param pConnString
        format: @b server=<server>;database=<database>;Trusted_Connection=Yes (MSSQL connect string)
        WARNING: must be used only to connect, for buffer doesn't live long
*/
void SqlNativeDriver::connect(
                              char* pConnString,
                              SQL_Driver_services& rServices,
                              void** pConnection)
{
    Connection& rConnection = *static_cast<Connection*>(rServices.malloc(sizeof(Connection)));
    *pConnection = &rConnection;
    rConnection.services = &rServices;

    CStringA codePage(rServices.request_charset());
    codePage_ = codePage == "UTF-8" ? CP_UTF8 : CP_ACP;

    ConnectionString str(pConnString);
    str.Parse();

    try {
        rConnection.pClient = new Client(CStringW(str.GetOleDBString().c_str()));
    }
    catch(const std::exception& e) {
        _throw(rServices, e);
    }
}

void SqlNativeDriver::disconnect(void* pConnection)
{
    Connection& rConnection = *static_cast<Connection*>(pConnection);
    try {
        delete rConnection.pClient;
        rConnection.pClient = NULL;
    }
    catch(const std::exception&) {
        // do nothing
    }
}

void SqlNativeDriver::commit(void* p)
{
    UNREFERENCED_PARAMETER(p);
    // do nothing
}

void SqlNativeDriver::rollback(void* p)
{
    UNREFERENCED_PARAMETER(p);
    // do nothing
}

bool SqlNativeDriver::ping(void* p)
{
    UNREFERENCED_PARAMETER(p);
    return true;
}

const char* SqlNativeDriver::quote(
                                   void* pConnection,
                                   const char* from,
                                   unsigned int length)
{
    Connection& rConnection = *static_cast<Connection*>(pConnection);
    char* result = static_cast<char*>(rConnection.services->malloc_atomic(length*2+1));
    char* to = result;
    while(length--) {
        if(*from == '\'') {  // ' -> ''
            *to++='\'';
        }
        *to++=*from++;
    }
    *to = 0;
    return result;
}

void SqlNativeDriver::InsertColumnValue(
    const CStringA& rMultibyteStr,
    Connection& rConnection,
    SQL_Driver_query_event_handlers& rHandlers,
    SQL_Error& rSqlError)
{
    size_t length = rMultibyteStr.GetLength();

    char* pStr = static_cast<char*>(rConnection.services -> malloc_atomic(length+1));
    strcpy_s(pStr, length+1, rMultibyteStr);
    ThrowIfError(*rConnection.services, rHandlers.add_row_cell(rSqlError, pStr, length));
}

void SqlNativeDriver::query(
    void* pConnection,
    const char* pStatement,
    size_t nPlaceholders,
    Placeholder* pPlaceholders,
    unsigned long offset,
    unsigned long limit,
    SQL_Driver_query_event_handlers& rHandlers)
{
    Connection& rConnection = *static_cast<Connection*>(pConnection);
    Client* pClient = rConnection.pClient;
    SQL_Driver_services& rServices = *rConnection.services;

    CStringW sql = CA2W(pStatement, codePage_);

    LPCWSTR pSql = sql.GetString();
    QueryDecorator decorator(pSql, limit, offset, !IsNoLimit(limit));

    try {
        // Server cursors are created only for statements that begin with
        // SELECT
        // EXEC[ute] procedure_name
        if(!decorator.IsSelectQuery() && !decorator.IsExecQuery()) {
            pClient->ExecuteNonQuery(sql);
            return;
        }
        if(nPlaceholders > 0) {
            ExecuteReader(decorator, rConnection, rHandlers, offset, limit, nPlaceholders, pPlaceholders);
        } else {
            ExecuteReader(decorator, rConnection, rHandlers, offset, limit);
        }
    }
    catch(const wexception& e) {
        rServices._throw(CW2A(e.GetMessage()->c_str(), codePage_));
    }
    catch(const std::exception& e) {
        _throw(rServices, e);
    }
}

void SqlNativeDriver::ExecuteReader(
        const QueryDecorator& decorator,
        Connection& rConnection,
        SQL_Driver_query_event_handlers& rHandlers,
        unsigned long offset,
        unsigned long limit)
{
    Client* pClient = rConnection.pClient;
    SQL_Driver_services& rServices = *rConnection.services;

    CCommand<CDynamicStringAccessorW> cmd;
    pClient -> ExecuteReader(decorator.GetQuery(), &cmd);
    cmd.MoveFirst();
    DBORDINAL nColumns = cmd.GetColumnCount();
    ValidateColumns(&nColumns, rServices);
    SQL_Error sqlError;
    for(DBORDINAL i = 1; i <= nColumns; ++i) {
        CStringW name(cmd.GetColumnName(i));
        InsertColumn(name, rConnection, rHandlers, sqlError);
    }
    ReadResults(cmd, rConnection, rHandlers, offset, limit, nColumns, &cmd);
}

void SqlNativeDriver::ExecuteReader(
        const QueryDecorator& decorator,
        Connection& rConnection,
        SQL_Driver_query_event_handlers& rHandlers,
        unsigned long offset,
        unsigned long limit,
        size_t nPlaceholders,
        Placeholder* pPlaceholders)
{
    Client* pClient = rConnection.pClient;
    SQL_Driver_services& rServices = *rConnection.services;

    CCommand<CManualAccessor> cmd;

    std::map<CStringW, CStringW> parameters;
    for(size_t i = 0; i < nPlaceholders; ++i) {
        Placeholder& ph = pPlaceholders[i];
        CStringW sName = CA2W(ph.name, codePage_);
        CStringW sValue = CA2W(ph.value, codePage_);
        parameters[sName] = sValue;
    }

    pClient -> ExecuteReader(decorator.GetQuery(), &cmd, parameters);

    HRESULT result = NULL;

    ULONG nColumns = 0;
    CComHeapPtr<DBCOLUMNINFO> pColumnInfo;
    CComHeapPtr<OLECHAR> pStrings;

    result = cmd.GetColumnInfo(&nColumns, &pColumnInfo, &pStrings);

    ColumnBind* pBind = static_cast<ColumnBind*>(
        rConnection.services -> malloc_atomic(sizeof(ColumnBind) * nColumns));
    result = cmd.CreateAccessor(nColumns, &pBind[0], sizeof(ColumnBind) * nColumns);

    ValidateColumns(&nColumns, rServices);

    SQL_Error sqlError;
    for (ULONG l = 0; l < nColumns; ++l) {
        DBLENGTH sz = Client::CalculateStringLength(pColumnInfo[l].wType, pColumnInfo[l].ulColumnSize) + 1;
        pBind[l].nSize = sz;
        pBind[l].szValue = static_cast<LPWSTR>(rConnection.services -> malloc_atomic(sizeof(WCHAR) * sz));
        cmd.AddBindEntry(l+1, DBTYPE_WSTR, sizeof(WCHAR) * sz, pBind[l].szValue, NULL, &pBind[l].dwStatus);

        CStringW name(pColumnInfo[l].pwszName);
        InsertColumn(name, rConnection, rHandlers, sqlError);
    }
    cmd.Bind();

    result = cmd.MoveFirst();
    ReadResults(cmd, rConnection, rHandlers, offset, limit, nColumns, pBind);
}

void SqlNativeDriver::InsertColumn(
        const CStringW& rUnicodeStr,
        Connection& rConnection,
        SQL_Driver_query_event_handlers& rHandlers,
        SQL_Error& rSqlError
    )
{
    char *pStr = 0;
    size_t length = rUnicodeStr.GetLength();
    if(length) {
        pStr = static_cast<char*>(rConnection.services -> malloc_atomic(length+1));
        strcpy_s(pStr, length+1, CW2A(rUnicodeStr, codePage_));
    }
    ThrowIfError(*rConnection.services, rHandlers.add_column(rSqlError, pStr, length));
}

