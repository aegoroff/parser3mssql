// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#include <stdio.h>
#include <tchar.h>

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

#include <atlbase.h>
#include <atlstr.h>
#include <assert.h>
#include <windows.h>
#define _WIN32_DCOM
#include <string>
#include <iostream>
#include <sstream>
#include <memory>
#include <crtdbg.h>
#include <comdef.h>

#ifdef _DEBUG
# pragma comment(lib, "atlsd.lib")
#define _ATL_DEBUG_INTERFACES
#ifndef _CRTDBG_CHECK_ALWAYS_DF
#define _CRTDBG_CHECK_ALWAYS_DF
#endif
#endif

#include <sqlncli.h>
#include <atldbcli.h>

#pragma warning (push) // only this file
#pragma warning(disable : 4389) // some gtest warnings
#pragma warning(disable : 4722) // some gtest warnings
#include "gtest.h"
#pragma warning (pop)

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
	TypeName(const TypeName&);               \
	void operator=(const TypeName&)


