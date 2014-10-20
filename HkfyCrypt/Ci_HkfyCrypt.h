#ifndef _AFXSTD_99757EC1_B072_477D_B466_20F3B2919756_INCLUDE_H_
#define _AFXSTD_99757EC1_B072_477D_B466_20F3B2919756_INCLUDE_H_

#if _MSC_VER >= 1200
#pragma once
#endif //!_MSC_VER >= 1200

#include <string>
#include <fstream>
using namespace std;

//--------------------------------------------------------------------------
//								MD5 Encrypt
//--------------------------------------------------------------------------
typedef	class CMD5 *PCMD5;
class Ci_MD5 
{
public:
	Ci_MD5(const void* input, size_t length);
	Ci_MD5(const string& str);
	Ci_MD5(ifstream& in);
	~Ci_MD5();

public:
	const byte* Digest();
	string		ToString();

private:
	PCMD5	m_pMD5;
};


#endif //!_AFXSTD_99757EC1_B072_477D_B466_20F3B2919756_INCLUDE_H_