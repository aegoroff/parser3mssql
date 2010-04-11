/*
 * Copyright 2008-2009 Alexander Egorov <egoroff@gmail.com> (http://www.egoroff.spb.ru)
 */

#include "StdAfx.h"
#include "Client.h"

inline void Client::InitializeAndEstablishConnection() {
	CDBPropSet dbinit(DBPROPSET_DBINIT);
	bool result = dbinit.AddProperty(
			DBPROP_INIT_PROVIDERSTRING,
			connectionString_);
	if (!result) {
		throw std::exception("Failed to initialize connection");
    }
	HRESULT r = ds_.Open(CLSID_SQLNCLI, &dbinit);
	ThrowIfError(r, ds_.m_spInit, IID_IDBInitialize);
	session_.Open(ds_);
}

Client::Client(const CStringW& connectionString)
	: connectionString_(connectionString) {
	InitializeAndEstablishConnection();
}

Client::~Client() {
	session_.Close();
	ds_.Close();
}

void Client::ThrowIfError(
						  HRESULT result,
						  const CCommand<CNoAccessor>& cmd) const {
	ThrowIfError(result, cmd.m_spCommand, IID_ICommand);
}

void Client::ThrowIfError(
						  HRESULT result,
						  const CCommand<CManualAccessor>& cmd) const {
	ThrowIfError(result, cmd.m_spCommand, IID_ICommand);
}

void Client::ThrowIfError(
						  HRESULT result,
						  const CCommand<CDynamicStringAccessorW>& cmd) const {
	ThrowIfError(result, cmd.m_spCommand, IID_ICommand);
}

long Client::ExecuteNonQuery(LPCWSTR pStatement) {
	CCommand<CNoAccessor> cmd;
	cmd.CreateCommand(session_);
	DBROWCOUNT affected;
	HRESULT result = cmd.Open(
		session_,
		pStatement,
		0,
		&affected,
		DBGUID_DEFAULT,
		false);
	ThrowIfError(result, cmd);
	return affected;
};


void Client::ExecuteReader(
						   LPCWSTR pStatement,
						   CCommand<CManualAccessor>* pCmd,
						   const std::map<CStringW,
						   CStringW>& rParameters) {
	HRESULT result = NULL;
	pCmd ->Create(session_, pStatement);
	pCmd ->Prepare();

	std::map<CStringW, CStringW>::const_iterator it;

	result = pCmd ->CreateParameterAccessor(
		rParameters.size(),
		reinterpret_cast<void*>(0x01),
		0);
	ThrowIfError(result, *pCmd);

	for (it = rParameters.begin(); it != rParameters.end(); ++it) {
		size_t ix = _wtoi(it ->first);
		if (ix <= 0 || ix > rParameters.size()) {
			throw std::exception("bad bind parameter key");
		}
		pCmd ->AddParameterEntry(
			ix,
			DBTYPE_WSTR,
			it ->second.GetLength(),
			(void*)it ->second.GetString());
	}
	DBROWCOUNT affected;
	result = pCmd ->Open(NULL, &affected, true);
	ThrowIfError(result, *pCmd);
};

void Client::ExecuteReader(
						   LPCWSTR pStatement,
						   CCommand<CDynamicStringAccessorW>* pCmd) {
	DBROWCOUNT affected;
	HRESULT result = pCmd ->Open(session_, pStatement, 0, &affected);
	ThrowIfError(result, *pCmd);
};

DBLENGTH Client::CalculateStringLength(DBTYPE type, DBLENGTH size) {
	switch (type) {
		case DBTYPE_BOOL:			return 2;
		case DBTYPE_BYTES:			return size * 2;
		case DBTYPE_BSTR:			return size;
		case DBTYPE_STR:			return size;
		case DBTYPE_WSTR:			return size;
		case DBTYPE_I1:				return 5;
		case DBTYPE_I2:				return 7;
		case DBTYPE_I4:				return 12;
		case DBTYPE_I8:				return 22;
		case DBTYPE_UI1:			return 4;
		case DBTYPE_UI2:			return 6;
		case DBTYPE_UI4:			return 11;
		case DBTYPE_UI8:			return 21;
		case DBTYPE_R4:				return 13;
		case DBTYPE_R8:				return 23;		// maybe 9
		case DBTYPE_DECIMAL:		return 23;
		case DBTYPE_NUMERIC:		return 23;
		case DBTYPE_VARIANT:		return 20;
		case DBTYPE_IDISPATCH:		return 32;
		case DBTYPE_IUNKNOWN:		return 32;
		case DBTYPE_GUID:			return 38;
		case DBTYPE_ARRAY:			return 32;
		case DBTYPE_VECTOR:			return 32;
		case DBTYPE_DATE:			return 32;
		case DBTYPE_DBDATE:			return 32;
		case DBTYPE_DBTIME:			return 32;
		case DBTYPE_DBTIMESTAMP:	return 32;
		case DBTYPE_FILETIME:		return 32;
		case DBTYPE_PROPVARIANT:	return 32;
		case DBTYPE_VARNUMERIC:		return 32;
		case DBTYPE_CY:				return 32;
		default: {
			ATLASSERT(FALSE);  // unhandled column type
			return 32;
		}
	}
}
