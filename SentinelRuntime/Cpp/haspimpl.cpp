////////////////////////////////////////////////////////////////////
// Copyright (C) 2011, SafeNet, Inc. All rights reserved.
//
// HASP(R) is a registered trademark of SafeNet, Inc. 
//
//
// $Id: haspimpl.cpp,v 1.26 2011-05-06 06:18:50 nsingh1 Exp $
////////////////////////////////////////////////////////////////////
#include "hasp_api_cpp_.h"


////////////////////////////////////////////////////////////////////
// Construction/Destruction
////////////////////////////////////////////////////////////////////

ChaspImpl::ChaspImpl(hasp_feature_t feature, ChaspHandle& handle)
    : m_handle(0),
      m_feature(feature),
      m_ulState(stIdle),
      m_ulRefCount(0),
      m_ulAltered(0)
{
    addRef(handle);
}

ChaspImpl::~ChaspImpl()
{
    DIAG_ASSERT(0 == m_ulRefCount);

    logout(true);
    ChaspMap::removeKey(this);
}

////////////////////////////////////////////////////////////////////
// Implementation
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
//! Increments the internal reference counter and updates
//! the \a handle's \a ChaspHandle::m_ulAltered field.
////////////////////////////////////////////////////////////////////
bool ChaspImpl::addRef(ChaspHandle& handle)
{
    handle.m_ulAltered = m_ulAltered;
    return (0 < ++m_ulRefCount);
}

////////////////////////////////////////////////////////////////////
//! Returns the current altered counter.
////////////////////////////////////////////////////////////////////
hasp_u32_t ChaspImpl::altered() const
{
    return m_ulAltered;
}

////////////////////////////////////////////////////////////////////
//! Wrapper for the \a hasp_decrypt API function.
////////////////////////////////////////////////////////////////////
haspStatus ChaspImpl::decrypt(unsigned char* pData, 
                              hasp_size_t ulSize) const
{
    return isLoggedIn() ? 
                hasp_decrypt(m_handle, pData, ulSize) : 
                HASP_INV_HND;
}

////////////////////////////////////////////////////////////////////
//! Wrapper for the \a hasp_encrypt API function.
////////////////////////////////////////////////////////////////////
haspStatus ChaspImpl::encrypt(unsigned char* pData, 
                              hasp_size_t ulSize) const
{
    return isLoggedIn() ? 
                hasp_encrypt(m_handle, pData, ulSize) : 
                HASP_INV_HND;
}

////////////////////////////////////////////////////////////////////
//! Returns the feature this object represents.
////////////////////////////////////////////////////////////////////
hasp_feature_t ChaspImpl::feature() const
{
    DIAG_ASSERT(0 < m_ulRefCount);

    ChaspFeature feature(m_feature);
    return feature;
}

////////////////////////////////////////////////////////////////////
//! Wrapper for the \a hasp_get_size API function.
////////////////////////////////////////////////////////////////////
haspStatus ChaspImpl::getFileSize(hasp_fileid_t fileId, 
                                  hasp_size_t& ulSize) const
{
    return isLoggedIn() ?
                hasp_get_size(m_handle, fileId, &ulSize) : 
                HASP_INV_HND;
}

////////////////////////////////////////////////////////////////////
//! Wrapper for the \a hasp_get_rtc API function.
////////////////////////////////////////////////////////////////////
haspStatus ChaspImpl::getRtc(hasp_time_t& time) const
{
    return isLoggedIn() ? 
                hasp_get_rtc(m_handle, &time) : 
                HASP_INV_HND;
}

////////////////////////////////////////////////////////////////////
//! Wrapper  for the \a hasp_get_sessioninfo API function.
////////////////////////////////////////////////////////////////////
haspStatus ChaspImpl::getSessionInfo(const char* pszFormat, 
                                     ChaspInfo& info) const
{
    if (NULL == pszFormat)
        return HASP_INVALID_PARAMETER;

    info.clear();

    return isLoggedIn() ? 
                hasp_get_sessioninfo(m_handle, 
                                     const_cast<char*>(pszFormat),
                                     &info.m_pszInfo) :
                HASP_INV_HND;
}

////////////////////////////////////////////////////////////////////
//! Wrapper  for the \a hasp_get_info API function.
////////////////////////////////////////////////////////////////////
haspStatus ChaspImpl::getInfo(const char* pszQuery,
                              const char* pszFormat,
                              hasp_vendor_code_t vendorCode,
                              ChaspInfo& info)
{
    info.clear();

    return pszQuery && pszFormat ?
        hasp_get_info(const_cast<char*>(pszQuery),
                      const_cast<char*>(pszFormat),
                      vendorCode, 
                      &info.m_pszInfo) :
        HASP_INVALID_PARAMETER;
}

haspStatus ChaspImpl::detach(const char* pszAction, 
							 const char* pszScope, 
							 hasp_vendor_code_t vendorCode, 
							 const char* pszRecipient, 
							 ChaspInfo& v2c)
{
	v2c.clear();

	return pszAction && pszScope && pszRecipient ?
		hasp_detach(pszAction, pszScope, vendorCode, pszRecipient, &v2c.m_pszInfo) :
	    HASP_INVALID_PARAMETER;
}


haspStatus ChaspImpl::transfer(const char* pszAction, 
							 const char* pszScope, 
							 hasp_vendor_code_t vendorCode, 
							 const char* pszRecipient, 
							 ChaspInfo& v2c)
{
	v2c.clear();

	return pszAction && pszScope && pszRecipient ?
		hasp_transfer(pszAction, pszScope, vendorCode, pszRecipient, &v2c.m_pszInfo) :
	    HASP_INVALID_PARAMETER;
}
////////////////////////////////////////////////////////////////////
//! Wrapper for the \a hasp_get_version API function.
////////////////////////////////////////////////////////////////////
haspStatus ChaspImpl::getVersion(hasp_vendor_code_t vendorCode,
                                 ChaspVersion& version)
{
    unsigned int nMajor = 0;
    unsigned int nMinor = 0;
    unsigned int nServer = 0;
    unsigned int nBuildNo = 0;
    
    haspStatus status = hasp_get_version(&nMajor, 
                                         &nMinor, 
                                         &nServer, 
                                         &nBuildNo, 
                                         vendorCode);

    version = HASP_SUCCEEDED(status) ? 
        ChaspVersion(nMajor, nMinor, nServer, nBuildNo) :
        ChaspVersion();

    return status;
}

////////////////////////////////////////////////////////////////////
//! Returns the handle for the feature.
////////////////////////////////////////////////////////////////////
hasp_handle_t ChaspImpl::handle() const
{
    return isLoggedIn() ? m_handle : 0;
}

////////////////////////////////////////////////////////////////////
//! Used to check if a logout occured.
////////////////////////////////////////////////////////////////////
bool ChaspImpl::isAltered(const ChaspHandle& handle) const
{
    DIAG_ASSERT(0 < m_ulRefCount);

    return (0 >= m_ulRefCount) ||
           (m_ulAltered != handle.m_ulAltered);
}

////////////////////////////////////////////////////////////////////
//! Used to check if the object is in the login state.
////////////////////////////////////////////////////////////////////
bool ChaspImpl::isLoggedIn() const
{
    DIAG_ASSERT(0 < m_ulRefCount);
    DIAG_ASSERT(0 < ChaspMap::findKey(this));
    
    return (0 < m_ulRefCount) && (stActive == m_ulState);
}

////////////////////////////////////////////////////////////////////
//! Wrapper for the \a hasp_legacy_decrypt API function.
////////////////////////////////////////////////////////////////////
haspStatus ChaspImpl::legacyDecrypt(unsigned char* pData, 
                                    hasp_size_t ulSize)const
{
    return isLoggedIn() ? 
                hasp_legacy_decrypt(m_handle, pData, ulSize) : 
                HASP_INV_HND;
}

////////////////////////////////////////////////////////////////////
//! Wrapper for the \a hasp_legacy_encrypt API function.
////////////////////////////////////////////////////////////////////
haspStatus ChaspImpl::legacyEncrypt(unsigned char* pData, 
                                    hasp_size_t ulSize) const
{
    return isLoggedIn() ?
                hasp_legacy_encrypt(m_handle, pData, ulSize) : 
                HASP_INV_HND;
}

////////////////////////////////////////////////////////////////////
//! Wrapper  for the \a hasp_legacy_set_rtc API function.
////////////////////////////////////////////////////////////////////
haspStatus ChaspImpl::legacySetRtc(hasp_time_t time) const
{
    return isLoggedIn() ?
                hasp_legacy_set_rtc(m_handle, time) : 
                HASP_INV_HND;
}

////////////////////////////////////////////////////////////////////
//! Wrapper for the \a hasp_login_scope API function.
////////////////////////////////////////////////////////////////////
haspStatus ChaspImpl::login(hasp_vendor_code_t vendorCode,  const char* pszScope)
{
    DIAG_ASSERT(0 < m_ulRefCount);
    if (0 >= m_ulRefCount)
        return HASP_INVALID_OBJECT;

    if (isLoggedIn())
        return HASP_ALREADY_LOGGED_IN;

    haspStatus status = pszScope ? 
        hasp_login_scope(m_feature, const_cast<char*>(pszScope), vendorCode, &m_handle) :
        hasp_login(m_feature, vendorCode, &m_handle);

    if (HASP_SUCCEEDED(status))
        m_ulState = stActive;

    return status;
}

////////////////////////////////////////////////////////////////////
//! Wrapper for the \a hasp_logout API function.
////////////////////////////////////////////////////////////////////
haspStatus ChaspImpl::logout(bool bFinal /* = false */)
{
    // final call from dtor
    // Ref. counter shall be zero.
    // Perform logout when still active.
    if (bFinal)
    {
        DIAG_ASSERT(0 == m_ulRefCount);
        if (stActive == m_ulState)
        {
            m_ulState = stIdle;
            hasp_logout(m_handle);
        }

        // nothing left to do.
        return HASP_STATUS_OK;
    }

    DIAG_ASSERT(0 < m_ulRefCount);
    if (0 >= m_ulRefCount)
        return HASP_INVALID_OBJECT;

    if (!isLoggedIn())
        return HASP_ALREADY_LOGGED_OUT;

    haspStatus status = hasp_logout(m_handle);

    if (HASP_SUCCEEDED(status))
    {
        // alter the state and update 
        // the logout counter.
        m_ulState = stIdle;
        m_ulAltered++;
    }

    return status;
}

////////////////////////////////////////////////////////////////////
//! Releases the the object by decrementing its reference counter.
//! If the reference counter drops below one the object will be
//! destroyed.
////////////////////////////////////////////////////////////////////
bool ChaspImpl::release()
{
    DIAG_ASSERT(0 < m_ulRefCount);
    if (0 == m_ulRefCount)
        return false;

    if (0 == --m_ulRefCount)
        // gone...
        delete this;

    return true;
}

////////////////////////////////////////////////////////////////////
//! Wrapper for the \a hasp_read API function.
////////////////////////////////////////////////////////////////////
haspStatus ChaspImpl::readFile(hasp_fileid_t fileId,
                               hasp_size_t ulOffset,
                               unsigned char* pData, 
                               hasp_size_t ulSize) const
{
    return isLoggedIn() ?
                hasp_read(m_handle, 
                         fileId, 
                         ulOffset, 
                         ulSize, 
                         pData) :
                HASP_INV_HND;
}

////////////////////////////////////////////////////////////////////
//! Wrapper for the \a hasp_legacy_set_idletime API function.
////////////////////////////////////////////////////////////////////
haspStatus ChaspImpl::setIdleTime(hasp_u16_t nIdleTime) const
{
    return isLoggedIn() ? 
                hasp_legacy_set_idletime(m_handle, nIdleTime) :
                HASP_INV_HND;
}

////////////////////////////////////////////////////////////////////
//! Returns the handle as a human readable string.
////////////////////////////////////////////////////////////////////
std::string ChaspImpl::toString() const
{
    if (!isLoggedIn())
        return std::string();

    std::ostringstream stream;
    stream << m_handle;

    return std::string(stream.str());
}

////////////////////////////////////////////////////////////////////
//! Wrapper for the \a hasp_write API function.
////////////////////////////////////////////////////////////////////
haspStatus ChaspImpl::writeFile(hasp_fileid_t fileId,
                                hasp_size_t ulOffset,
                                const unsigned char* pData, 
                                hasp_size_t ulSize) const
{
    if (!isLoggedIn())
        return HASP_INV_HND;

    return isLoggedIn() ?
                hasp_write(m_handle, 
                           fileId, 
                           ulOffset, 
                           ulSize, 
                           const_cast<unsigned char*>(pData)) :
                HASP_INV_HND;
}

////////////////////////////////////////////////////////////////////
//! Wrapper for the \a hasp_update API function.
////////////////////////////////////////////////////////////////////
haspStatus ChaspImpl::update(const char* pszUpdate, 
                             ChaspInfo& acknowledge)
{
    if (NULL == pszUpdate)
        return HASP_INVALID_PARAMETER;

    acknowledge.clear();

    return hasp_update(const_cast<char*>(pszUpdate), 
                       &acknowledge.m_pszInfo);
}
