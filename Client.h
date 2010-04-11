/*
 * Copyright 2008-2009 Alexander Egorov <egoroff@gmail.com> (http://www.egoroff.spb.ru)
 */

#pragma once
#include "ComSession.h"

/**
* \brief Represents an OLE DB client.
*/
class Client : public ComSession {
 public:
		/**
		* \brief Creates new Client instance.
		* @param connectionString SQL server connection string
		*/
		explicit Client(const CStringW& connectionString);

		/**
		* \brief Virtual descructor
		*/
		virtual ~Client();

		/**
		* \brief Executes a query that returns nothing
		* @param pStatement SQL statement pointer
		*/
		long ExecuteNonQuery(__in LPCWSTR pStatement);

		/**
		* \brief Executes a parameterized query that returns data table
		* @param pStatement SQL statement pointer
		* @param pCmd SQL command opbject
		* @param rParameters Query parameterd
		*/
		void ExecuteReader(
			__in LPCWSTR pStatement,
			__deref_in CCommand<CManualAccessor>* pCmd,
			const std::map<CStringW,
			CStringW>& rParameters);

		/**
		* \brief Executes a query that returns data table
		* @param pStatement SQL statement pointer
		* @param pCmd SQL command opbject
		*/
		void ExecuteReader(
			__in LPCWSTR pStatement,
			__deref_in CCommand<CDynamicStringAccessorW>* pCmd);

		/**
		* \brief Calculates the length of string representation of a type
		* @param type Type which string length representation to calculate
		* @param size DB field size. Used for string and some other types.
		* \return The max possible string representation length of a type
		*/
		static DBLENGTH CalculateStringLength(DBTYPE type, DBLENGTH size);
 private:
		CSession session_;
		CDataSource ds_;
		const CStringW& connectionString_;

		void InitializeAndEstablishConnection();
		void ThrowIfError(HRESULT, const CCommand<CNoAccessor>&) const;
		void ThrowIfError(HRESULT, const CCommand<CManualAccessor>&) const;
		void ThrowIfError(HRESULT, const CCommand<CDynamicStringAccessorW>&) const;
		using ComSession::ThrowIfError;
		DISALLOW_COPY_AND_ASSIGN(Client);
};
