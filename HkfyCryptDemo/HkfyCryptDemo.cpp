#include "stdafx.h"
#include "HkfyCryptAPI.h"

#pragma comment(lib, "../bin/HkfyCrypt.lib")

int _tmain(int argc, _TCHAR* argv[])
{
	TCHAR szEncrypt[33] = {0};

	MD5_GetFile(L"C:\\Users\\Administrator\\Desktop\\12.txt", szEncrypt, 33);

	return 0;
}

