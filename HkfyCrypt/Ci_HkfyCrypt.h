#ifndef _AFXSTD_99757EC1_B072_477D_B466_20F3B2919756_INCLUDE_H_
#define _AFXSTD_99757EC1_B072_477D_B466_20F3B2919756_INCLUDE_H_

#if _MSC_VER >= 1200
#pragma once
#endif //!_MSC_VER >= 1200


//--------------------------------------------------------------------------
//								MD5 Encrypt
//--------------------------------------------------------------------------
typedef	class CRSA *PCRSA;
class Ci_RSA 
{
public:
	Ci_RSA(const void* input, size_t length);
	Ci_RSA(const string& str);
	Ci_RSA(ifstream& in);
	~Ci_RSA();

public:
	const byte* Digest();
	string		ToString();

private:
	PCRSA	m_pMD5;
};

//--------------------------------------------------------------------------
//								Sentinel Encrypt
//--------------------------------------------------------------------------
typedef class CSentinel  *PCSENTINEL;
class Ci_HKFY
{
public:
	Ci_HKFY();
	~Ci_HKFY();

public:
	const char *GetErrorName(int errorCode);

	int  LogIn(unsigned long lFeatureId);
	int  LogOut();

	int  CheckRom();
	int  CheckRam();

	int  GetPointNum();

private:
	PCSENTINEL	m_pSentinel;
};


#endif //!_AFXSTD_99757EC1_B072_477D_B466_20F3B2919756_INCLUDE_H_