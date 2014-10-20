#ifndef _AFXSTD_EFD0B66D_27F9_4178_9579_0A66D59E6348_INCLUDE_H_
#define _AFXSTD_EFD0B66D_27F9_4178_9579_0A66D59E6348_INCLUDE_H_

#if _MSC_VER >= 1200
#pragma once
#endif //!_MSC_VER >= 1200

#ifndef HKFYCRYPT_EXPORTS
	#define HKFYCRYPT_EXPORTS_API extern "C" _declspec(dllimport)
#else
	#define HKFYCRYPT_EXPORTS_API extern "C" _declspec(dllexport)
#endif //!HKFYCRYPT_EXPORTS


#define  MD5_ENCRYPT_LEN	32+1	//文件的MD5的长度


//=================================MD5_GetFile()================================
/// @brief 文件的MD5码
///
/// 根据文件的内容生成相应的MD5码,用于验证文件是否发生修改
///
/// @param [in ] pSorFilePath  源文件的路径
/// @param [out] pSzEncrypt	   返出文件的MD5码
/// @param [in ] pSzEncrypt	   MD5码的长度
///
/// @return 成功返回0
//================================================================================
HKFYCRYPT_EXPORTS_API int MD5_GetFile(TCHAR *pSorFilePath, TCHAR *pSzEncrypt, int len=MD5_ENCRYPT_LEN);


#endif //!_AFXSTD_5F8D5465_E219_4070_B0F1_5A3A0DD4CECD_INCLUDE_H_