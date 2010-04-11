/*
 * Copyright 2008-2009 Alexander Egorov <egoroff@gmail.com> (http://www.egoroff.spb.ru)
 */

#pragma once

/**
* \brief Represents a COM session abstruction. Contains different COM infrastructure helpers methods.
*/
class ComSession {
 public:
        /**
        * \brief Creates new ComSession instance.
        */
        ComSession();

        /**
        * \brief Virtual descructor
        */
        virtual ~ComSession();

 protected:
        /**
        * \brief Just calls CoUninitialize();
        */
        virtual void Clear();

        /**
        * \brief Checks HRESULT and throws exception in case of an error (bad HRESULT)
        * @param result HRESULT to check
        * @param pMessage Error message
        */
        void ThrowIfError(HRESULT result, __in LPCWSTR pMessage) const;

        /**
        * \brief Checks HRESULT and throws exception in case of an error (bad HRESULT)
        * @param result HRESULT to check
        * @param pObjectWithError Checked object
        * @param IID_InterfaceWithError Checked object's interface
        */
        void ThrowIfError(
            HRESULT result,
            __in IUnknown* pObjectWithError,
            REFIID IID_InterfaceWithError) const;
 private:
    void DumpErrorInfo(IUnknown*, REFIID) const;

    /**
    * \brief DumpErrorInfo queries SQLOLEDB error interfaces, retrieving available  status or error information.
    */
    void DumpErrorInfo(__in std::wostream*, IUnknown*, REFIID) const;
    DISALLOW_COPY_AND_ASSIGN(ComSession);
};

