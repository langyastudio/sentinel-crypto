#ifndef _AFXSTD_17B37EDA_AA90_4982_8F24_91FE144BFA06_INCLUDE_H_
#define _AFXSTD_17B37EDA_AA90_4982_8F24_91FE144BFA06_INCLUDE_H_

#if _MSC_VER >= 1200
#pragma once
#endif//!_MSC_VER >= 1200

#include "Errorprinter.h"

//--------------------------------------------------------------------------
//								Sentinel Encrypt
//--------------------------------------------------------------------------
class CSentinel
{
public:
	CSentinel();
	~CSentinel();

public:
	const char *GetErrorName(int errorCode);
	
	int  LogIn(unsigned long lFeatureId);
	int  LogOut();
	
#define HASP_ROM_LEN  112
#define HASP_RAM_LEN  112
	int  CheckRom();
	int  CheckRam();

	int  GetPointNum();

private:
	int  DecVendor();

	int  GetHLInfo(string &strHLId,  string &strHLType);
	int  GetUniMachineCode(string &strCode);

	int  DecMachineNum(unsigned char  szSorMachine[16], unsigned char  szDesMachine[16], int &num);
	int  EncMachineNum(unsigned char  szSorMachine[16], unsigned char  szDesMachine[16]);

private:
	hasp_handle_t	m_pChasp;
	CErrorPrinter	*m_pError;	
	unsigned char   *m_pCode;
	unsigned char	 m_pChannels;
};


#endif //!_AFXSTD_17B37EDA_AA90_4982_8F24_91FE144BFA06_INCLUDE_H_