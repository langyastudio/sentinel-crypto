#include "stdafx.h"
#include "Ci_HkfyCrypt.h"
#include "MD5.h"
#include "Sentinel.h"

//--------------------------------------------------------------------------
//								MD5 Encrypt
//--------------------------------------------------------------------------
Ci_RSA::Ci_RSA( const void* input, size_t length )
{
	m_pMD5 = new CRSA(input, length);
}

Ci_RSA::Ci_RSA( const string& str )
{
	m_pMD5 = new CRSA(str);
}

Ci_RSA::Ci_RSA( ifstream& in )
{
	m_pMD5 = new CRSA(in);
}

Ci_RSA::~Ci_RSA()
{
	delete (CRSA*)m_pMD5;
}

const byte* Ci_RSA::Digest()
{
	return(m_pMD5->digest());
}

std::string Ci_RSA::ToString()
{
	return(m_pMD5->toString());
}

//--------------------------------------------------------------------------
//								Sentinel Encrypt
//--------------------------------------------------------------------------
Ci_HKFY::Ci_HKFY()
{
	m_pSentinel = new CSentinel();
}

Ci_HKFY::~Ci_HKFY()
{
	delete m_pSentinel;
}

const char * Ci_HKFY::GetErrorName( int errorCode )
{
	return(m_pSentinel->GetErrorName(errorCode));
}

int Ci_HKFY::LogIn( unsigned long lFeatureId )
{
	return(m_pSentinel->LogIn(lFeatureId));
}

int Ci_HKFY::LogOut()
{
	return(m_pSentinel->LogOut());
}

int Ci_HKFY::CheckRom()
{
	return(m_pSentinel->CheckRom());
}

int Ci_HKFY::CheckRam()
{
	return(m_pSentinel->CheckRam());
}

int  Ci_HKFY::GetPointNum()
{
	return(m_pSentinel->GetPointNum());
}
