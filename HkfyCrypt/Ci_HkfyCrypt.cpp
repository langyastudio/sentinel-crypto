#include "stdafx.h"
#include "Ci_HkfyCrypt.h"
#include "MD5.h"

//--------------------------------------------------------------------------
//								MD5 Encrypt
//--------------------------------------------------------------------------
Ci_MD5::Ci_MD5( const void* input, size_t length )
{
	m_pMD5 = new CMD5(input, length);
}

Ci_MD5::Ci_MD5( const string& str )
{
	m_pMD5 = new CMD5(str);
}

Ci_MD5::Ci_MD5( ifstream& in )
{
	m_pMD5 = new CMD5(in);
}

Ci_MD5::~Ci_MD5()
{
	delete (CMD5*)m_pMD5;
}

const byte* Ci_MD5::Digest()
{
	return(m_pMD5->digest());
}

std::string Ci_MD5::ToString()
{
	return(m_pMD5->toString());
}
