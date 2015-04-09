#include "stdafx.h"
#include <HkfyCryptAPI.h>

#pragma comment(lib, "HkfyCrypt.lib")

int _tmain(int argc, _TCHAR* argv[])
{
	CHKFY crypt;

	int  statue = crypt.LogIn(7);
	if (statue != 0)
	{
		goto END;
	}

	statue = crypt.CheckRam();
	if (statue != 0)
	{
		goto END;
	}

	statue = crypt.CheckRom();
	if (statue != 0)
	{
		goto END;
	}

	statue = crypt.GetPointNum();
	
END:
	return 0;
}



