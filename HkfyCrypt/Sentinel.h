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
	virtual ~CSentinel();

public:
	int  LogIn(unsigned long lFeatureId);
    const char *GetErrorName(int errorCode);
	int  LogOut();
	
#define HASP_ROM_LEN  112
#define HASP_RAM_LEN  112
	int  CheckRom();
	int  CheckRam();
    int  ClearRam();

	int  GetPointNum();

private:
	int  DecVendor(); // 解码开发商代码

	int  GetHLInfo(string &strHLId,  string &strHLType); // 获取加密狗信息
	int  GetUniMachineCode(string &strCode); // 获取唯一机器码
    
	int  DecMachineNum(unsigned char  szSorMachine[16], unsigned char  szDesMachine[16], int &num); // 解码机器号
	int  EncMachineNum(unsigned char  szSorMachine[16], unsigned char  szDesMachine[16]); // 编码机器号

private:
	hasp_handle_t	 m_pChasp;
	CErrorPrinter	*m_pError;	
	unsigned char   *m_pCode;
	char	         m_pChannels;
};


#endif //!_AFXSTD_17B37EDA_AA90_4982_8F24_91FE144BFA06_INCLUDE_H_