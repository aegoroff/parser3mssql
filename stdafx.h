// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#pragma warning( disable : 4290 )

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif

#include <stdio.h>
#include <tchar.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <memory>


#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

#include <atlbase.h>
#include <atlstr.h>
#include <assert.h>
#include <windows.h>
#define _WIN32_DCOM
#include <string>
#include <iostream>
#include <sstream>
#include <map>
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
#include <atldbsch.h>  // For Rowset Schema Information

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
	TypeName(const TypeName&);               \
	void operator=(const TypeName&)
