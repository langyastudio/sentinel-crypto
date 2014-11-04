#ifndef _AFXSTD_6B5BFA3D_3912_4277_957A_982D725AEFE3_INCLUDE_H_
#define _AFXSTD_6B5BFA3D_3912_4277_957A_982D725AEFE3_INCLUDE_H_

#if _MSC_VER >= 1200
#pragma once
#endif//!_MSC_VER >= 1200


//--------------------------------------------------------------------------
//								´íÎóÂëÓ³Éä
//--------------------------------------------------------------------------
class CErrorPrinter
{
public:
    CErrorPrinter();

public:
    const char* GetError(hasp_status_t status);
	void PrintError(hasp_status_t status);

    typedef std::map<hasp_status_t, const char*> StatusMap;

private:
    StatusMap errorMap;
};

#endif //!_AFXSTD_6B5BFA3D_3912_4277_957A_982D725AEFE3_INCLUDE_H_