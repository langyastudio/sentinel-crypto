////////////////////////////////////////////////////////////////////
// Copyright (C) 2011, SafeNet, Inc. All rights reserved.
//
// HASP(R) is a registered trademark of SafeNet, Inc. 
//
//
// $Id: hasp_api_cpp_.h,v 1.12 2011-05-06 06:18:50 nsingh1 Exp $
////////////////////////////////////////////////////////////////////
#if !defined(__HASP_API_CPP_PRIVATE_H__)
#define __HASP_API_CPP_PRIVATE_H__


#include "hasp_api_cpp.h"
#include <sstream>

////////////////////////////////////////////////////////////////////
// Diagnostics support
////////////////////////////////////////////////////////////////////
#include "haspdiag.h"

////////////////////////////////////////////////////////////////////
// foreward declarations
////////////////////////////////////////////////////////////////////
struct ChaspMapEntry;
struct ChaspLock;
class ChaspMapImpl;
class ChaspMap;
class ChaspImpl;


////////////////////////////////////////////////////////////////////
// struct ChaspLock
////////////////////////////////////////////////////////////////////

#if defined(WIN32) || defined(WIN64)
    #define HASP_HL_TARGET_WINDOWS
#endif // WIN32 || WIN64

////////////////////////////////////////////////////////////////////
// Windows
////////////////////////////////////////////////////////////////////
#if defined(HASP_HL_TARGET_WINDOWS)

// exclude rarely used window stuff
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

struct ChaspLock
{
// Construction/Destruction
public:
    ChaspLock();
    ~ChaspLock();

// Attributes
protected:
    CRITICAL_SECTION                        m_critLock;
    bool                                    m_bInit;

// Implementation
public:
    bool isInit() const;
    bool lock();
    bool unlock();
};

#else
////////////////////////////////////////////////////////////////////
// Unix/Mac
////////////////////////////////////////////////////////////////////
#include <pthread.h>

struct ChaspLock
{
// Construction/Destruction
public:
    ChaspLock();
    ~ChaspLock();

// Attributes
protected:
    pthread_mutexattr_t                     m_attrRecursive;
    pthread_mutex_t                         m_mutex;
    bool                                    m_bInit;

// Implementation
public:
    bool isInit() const;
    bool lock();
    bool unlock();
};

#endif // HASP_HL_TARGET_WINDOWS


////////////////////////////////////////////////////////////////////
// struct ChaspMapEntry
////////////////////////////////////////////////////////////////////
struct ChaspMapEntry
{
// Construction/Destruction
public:
    ChaspMapEntry();
    ~ChaspMapEntry();

// Attributes
public:
    hasp_u32_t                              m_ulCount;
    ChaspImpl*                              m_pKey;
};


////////////////////////////////////////////////////////////////////
// class ChaspMapImpl
////////////////////////////////////////////////////////////////////
class ChaspMapImpl
{
// Construction/Destruction
public:
    ChaspMapImpl();
    ~ChaspMapImpl();

// Attributes
public:
    static const hasp_u32_t                 m_ulInitCapacity;
    std::vector<struct ChaspMapEntry>       m_map;
    ChaspLock                               m_lock;

// Implementation
public:
    bool addRef(ChaspHandle& handle) const;
    bool createKey(hasp_u32_t feature, ChaspHandle& handle);
    hasp_u32_t findKey(const ChaspImpl* pKey);
    ChaspImpl* getKey(const ChaspHandle& handle) const;
    bool lockMap() const;
    bool release(ChaspHandle& handle) const;
    bool removeKey(const ChaspImpl* pKey);
    bool unlockMap() const;
};


////////////////////////////////////////////////////////////////////
// class ChaspMap
////////////////////////////////////////////////////////////////////
class ChaspMap
{
// Construction/Destruction
public:
    ChaspMap();
    ~ChaspMap();

// Attributes
protected:
    bool                                    m_bLocked;

// Implementation
public:
    static bool addRef(ChaspHandle& handle);
    static bool createKey(hasp_u32_t feature, ChaspHandle& handle);
    static hasp_u32_t findKey(const ChaspImpl* pKey);
    ChaspImpl* getKey(const ChaspHandle& handle) const;
    static bool release(ChaspHandle& handle);
    static bool removeKey(const ChaspImpl* pKey);

protected:
    friend class ChaspBase;
    static ChaspMapImpl& map();
};


////////////////////////////////////////////////////////////////////
// helper macros
////////////////////////////////////////////////////////////////////
#define HASP_PROLOGUE(handle) \
    ChaspMap _map; \
    DIAG_ASSERT(isValid()); \
    if (!isValid()) \
        return HASP_INVALID_OBJECT; \
    ChaspImpl* _pKey = _map.getKey((handle)); \
    DIAG_ASSERT(NULL != _pKey); \
    if (NULL == _pKey) \
        return HASP_INVALID_OBJECT; \

#define HASP_KEYPTR                         _pKey


////////////////////////////////////////////////////////////////////
// class ChaspImpl
////////////////////////////////////////////////////////////////////
class ChaspImpl
{
// Construction/Destruction
public:
    ChaspImpl(hasp_feature_t feature, ChaspHandle& handle);

protected:
    virtual ~ChaspImpl();

// Attributes
protected:
    enum
    {
        stIdle                              = 0,
        stActive                            = 1
    };

    hasp_handle_t                           m_handle;
    hasp_feature_t                          m_feature;
    hasp_u32_t                              m_ulState;
    hasp_u32_t                              m_ulRefCount;
    hasp_u32_t                              m_ulAltered;

// Implementation
public:
    hasp_u32_t altered() const;
    haspStatus decrypt(unsigned char* pData, hasp_size_t ulSize) const;
    haspStatus encrypt(unsigned char* pData, hasp_size_t ulSize) const;
    hasp_feature_t feature() const;
    haspStatus getFileSize(hasp_fileid_t fileId, hasp_size_t& ulSize) const;
    static haspStatus getInfo(const char* pszQuery, const char* pszFormat, hasp_vendor_code_t vendorCode, ChaspInfo& info);
    static haspStatus detach(const char* pszAction, const char* pszScope, 
							hasp_vendor_code_t vendorCode, const char* pszRecipient, ChaspInfo& v2c);
    static haspStatus transfer(const char* pszAction, const char* pszScope, 
							hasp_vendor_code_t vendorCode, const char* pszRecipient, ChaspInfo& v2c);
    haspStatus getRtc(hasp_time_t& time) const;
    haspStatus getSessionInfo(const char* pszFormat, ChaspInfo& info) const;
    static haspStatus getVersion(hasp_vendor_code_t vendorCode, ChaspVersion& version);
    hasp_handle_t handle() const;
    bool isAltered(const ChaspHandle& handle) const;
    bool isLoggedIn() const;
    haspStatus legacyEncrypt(unsigned char* pData, hasp_size_t ulSize) const;
    haspStatus legacyDecrypt(unsigned char* pData, hasp_size_t ulSize)const;
    haspStatus legacySetRtc(hasp_time_t time) const;
    haspStatus login(hasp_vendor_code_t vendorCode, const char* pszScope );
    haspStatus logout(bool bFinal = false);
    haspStatus readFile(hasp_fileid_t fileId,
                        hasp_size_t ulOffset,
                        unsigned char* pData,
                        hasp_size_t ulSize) const;
    haspStatus setIdleTime(hasp_u16_t nIdleTime) const;
    std::string toString() const;
    haspStatus writeFile(hasp_fileid_t fileId,
                         hasp_size_t ulOffset,
                         const unsigned char* pData,
                         hasp_size_t ulSize) const;
    static haspStatus update(const char* pszUpdate, ChaspInfo& acknowledge);

public:
    bool addRef(ChaspHandle& handle);
    bool release();
};


////////////////////////////////////////////////////////////////////
// class ChaspBase64
////////////////////////////////////////////////////////////////////
class ChaspBase64
{
// Construction/Destruction
public:
    ChaspBase64();
    ~ChaspBase64();

// Attributes
protected:
    static const char                       m_rAlphabet[];
    static const char                       m_cPad;
    static const size_t                     m_ulMaxLineLength;

// Implementation
public:
    static void decode(const std::string& source,
                       std::vector<unsigned char>& dest) throw();
    static void encode(const std::vector<unsigned char>& source,
                       std::string& dest) throw();

protected:
    int decodeChar(const int nChar) const;
};


#endif // !__HASP_API_CPP_PRIVATE_H__
