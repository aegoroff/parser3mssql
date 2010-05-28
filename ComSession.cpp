/*!
 * \brief   The file contains COM session abstruction implementation
 * \author  \verbatim
            Created by: Alexander Egorov
            \endverbatim
 * \date    \verbatim
            Creation date: 2010-04-11
            \endverbatim
 * Copyright 2008-2009 Alexander Egorov <egoroff@gmail.com> (http://www.egoroff.spb.ru)
 */

#include "StdAfx.h"
#include "ComSession.h"
#include "wexception.h"

inline void WriteErrorMessage(
    CComPtr<IErrorInfo>        pIErrorInfo,
    __deref_out std::wostream* pOstr)
{
    // IErrorInfo values.
    CComBSTR bstrDescription;
    CComBSTR bstrSource;

    pIErrorInfo->GetSource(&bstrSource);
    pIErrorInfo->GetDescription(&bstrDescription);

    if (bstrSource != NULL) {
        *pOstr << L"SOURCE:\t\t" << bstrSource.m_str << std::endl;
    }
    if (bstrDescription != NULL) {
        *pOstr
        << L"ERROR MESSAGE:\t"
        << bstrDescription.m_str
        << std::endl
        << std::endl;
    }
}

// DumpErrorInfo queries SQLOLEDB error interfaces, retrieving available
// status or error information.
inline void ComSession::DumpErrorInfo(
    std::wostream* pOstr,
    IUnknown*      pObjectWithError,
    REFIID         rErrorInterface) const
{
    // Interfaces used in the example.
    CComPtr<IErrorInfo> pIErrorInfoAll;
    CComPtr<IErrorRecords> pIErrorRecords;
    CComPtr<ISupportErrorInfo> pISupportErrorInfo;

    // Only ask for error information if the interface supports
    // it.
    if (FAILED(pObjectWithError->QueryInterface(IID_ISupportErrorInfo, reinterpret_cast<void**>(&pISupportErrorInfo)))) {
        *pOstr << L"SupportErrorErrorInfo interface not supported" << std::endl;
        return;
    }
    if (FAILED(pISupportErrorInfo->InterfaceSupportsErrorInfo(rErrorInterface))) {
        *pOstr << L"InterfaceWithError interface not supported" << std::endl;
        return;
    }

    // Do not test the return of GetErrorInfo. It can succeed and return
    // a NULL pointer in pIErrorInfoAll. Simply test the pointer.
    HRESULT r = GetErrorInfo(0, &pIErrorInfoAll);

    if ((pIErrorInfoAll == NULL) || FAILED(r)) {
        *pOstr << L"GetErrorInfo failed." << std::endl;
        return;
    }
    // Test to see if it's a valid OLE DB IErrorInfo interface
    // exposing a list of records.
    if (FAILED(pIErrorInfoAll->QueryInterface(IID_IErrorRecords, reinterpret_cast<void**>(&pIErrorRecords))) ) {
        // IErrorInfo is valid; get the source and
        // description to see what it is.
        WriteErrorMessage(pIErrorInfoAll, pOstr);
        return;
    }

    // Basic error information from GetBasicErrorInfo.
    ERRORINFO errorinfo;

    // Number of error records.
    ULONG nRecs;
    ULONG nRec;

    // ISQLErrorInfo parameters.
    CComBSTR bstrSQLSTATE;
    LONG lNativeError;

    pIErrorRecords->GetRecordCount(&nRecs);

    // Within each record, retrieve information from each
    // of the defined interfaces.
    for (nRec = 0; nRec < nRecs; ++nRec) {
        // From IErrorRecords, get the HRESULT and a reference
        // to the ISQLErrorInfo interface.
        pIErrorRecords->GetBasicErrorInfo(nRec, &errorinfo);

        CComPtr<ISQLErrorInfo> pISQLErrorInfo;
        CComPtr<IErrorInfo> pIErrorInfoRecord;

        pIErrorRecords->GetCustomErrorObject(
            nRec,
            IID_ISQLErrorInfo,
            reinterpret_cast<IUnknown**>(&pISQLErrorInfo));

        if (pISQLErrorInfo != NULL) {
            pISQLErrorInfo->GetSQLInfo(&bstrSQLSTATE, &lNativeError);

            // Display the SQLSTATE and native error values.
            *pOstr << L"SQLSTATE:\t" << bstrSQLSTATE.m_str << std::endl;
        }

        if (SUCCEEDED(pIErrorRecords->GetErrorInfo(nRec, ::GetSystemDefaultLCID(), &pIErrorInfoRecord))) {
            WriteErrorMessage(pIErrorInfoRecord, pOstr);
        }
    }
}

// DumpErrorInfo queries SQLOLEDB error interfaces, retrieving available
// status or error information.
inline void ComSession::DumpErrorInfo(
    IUnknown* pObjectWithError,
    REFIID    rErrorInterface) const
{
    DumpErrorInfo(&std::wcout, pObjectWithError, rErrorInterface);
}

ComSession::ComSession()
{
    HRESULT hr = CoInitializeEx(0, COINIT_MULTITHREADED);
    ThrowIfError(hr, L"Failed to initialize COM library");
}

ComSession::~ComSession()
{
    Clear();
    ATLASSERT(_CrtCheckMemory());
}

void ComSession::Clear()
{
    CoUninitialize();
}

void ComSession::ThrowIfError(HRESULT result, LPCWSTR pMessage) const
{
    if (FAILED(result)) {
        throw wexception(pMessage);
    }
}

void ComSession::ThrowIfError(
    HRESULT   result,
    IUnknown* pObjectWithError,
    REFIID    rErrorInterface) const
{
    if (FAILED(result)) {
        std::auto_ptr<std::wostringstream> msg(new std::wostringstream);
        DumpErrorInfo(msg.get(), pObjectWithError, rErrorInterface);
        throw wexception(msg->str().c_str());
    }
}

