// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <windows.h>


#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit


//自定义的库
#include <stdio.h>
#include <map>
#include <string>
#include <fstream>
#include <iostream>

#include <hasp_api.h>


using namespace std;

typedef unsigned char byte;
typedef unsigned int  uint32;
