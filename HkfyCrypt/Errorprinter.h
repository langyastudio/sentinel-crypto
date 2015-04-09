#ifndef _AFXSTD_6B5BFA3D_3912_4277_957A_982D725AEFE3_INCLUDE_H_
#define _AFXSTD_6B5BFA3D_3912_4277_957A_982D725AEFE3_INCLUDE_H_

#if _MSC_VER >= 1200
#pragma once
#endif//!_MSC_VER >= 1200


//--------------------------------------------------------------------------
//								º”√‹π∑¥ÌŒÛ¬Î”≥…‰
//--------------------------------------------------------------------------
class CErrorPrinter
{
public:
    CErrorPrinter();

public:
    const char* GetError(hasp_status_t status);
	void PrintError(hasp_status_t status);

private:
    typedef std::map<hasp_status_t, const char*> StatusMap;
    StatusMap       m_errorMap;
};

#endif //!_AFXSTD_6B5BFA3D_3912_4277_957A_982D725AEFE3_INCLUDE_H_