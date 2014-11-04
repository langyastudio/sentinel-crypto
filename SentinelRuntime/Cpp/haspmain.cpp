////////////////////////////////////////////////////////////////////
// Copyright (C) 2011, SafeNet, Inc. All rights reserved.
//
// HASP(R) is a registered trademark of SafeNet, Inc. 
//
//
// $Id: haspmain.cpp,v 1.11 2011-05-02 03:12:07 nsingh1 Exp $
////////////////////////////////////////////////////////////////////
#if defined(WIN32) && defined(_USRDLL)

#include "hasp_api_cpp_.h"


////////////////////////////////////////////////////////////////////
// DLL entry point
////////////////////////////////////////////////////////////////////

BOOL APIENTRY DllMain(HANDLE /* hModule */, 
                      DWORD  /* dwReason */, 
                      LPVOID /* lpReserved */)
{
    return TRUE;
}

#endif // WIN32 && _USRDLL
