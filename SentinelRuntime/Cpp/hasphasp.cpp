////////////////////////////////////////////////////////////////////
// Copyright (C) 2011, SafeNet, Inc. All rights reserved.
//
// HASP(R) is a registered trademark of SafeNet, Inc. 
//
//
// $Id: hasphasp.cpp,v 1.31 2011-08-12 07:20:04 syadav Exp $
////////////////////////////////////////////////////////////////////
#include "hasp_api_cpp_.h"


////////////////////////////////////////////////////////////////////
//! \class Chasp hasp_api_cpp.h
//! \brief Class for accessing HASP protection key.
//!
//! The Hasp class is used for accessing a HASP protection
//! key via a private key instance this class references.
//! This class is thread safe for reading and writing.
//!
//! Usage of this class is straight foreward.
//! \code
//!
//! // instantiate a class
//! Chasp hasp(ChaspFeature(10));
//! 
//! //verify that the object is valid
//! if (!isValid())
//!     return 0;
//!
//! // log in to a key
//! if (!HASP_SUCCEEDED(hasp.login(pVendor)))
//!     return 0;
//!
//! // ensure we are logged in
//! if (!isLoggedIn())
//!     return true;
//!
//! // create another instance pointing to the same key
//! Chasp haspOther = hasp;
//! // or
//! Chasp haspOther1;
//! haspOther1 = hasp;
//!
//! // must be the same
//! if ((haspOther != hasp) || (haspOther != haspOther1))
//!     return 0;
//!
//! // perform some file i/o
//! // therefore retrieve a file object
//! ChaspFile file = hasp.getFile(ChaspFile::fileMain);
//! if (!file.isValid())
//!     return false;
//!
//! // write some data
//! if (!(HASP_SUCCEEDED(file.setFilePos(10)) && 
//!       HASP_SUCCEEDED(file.writeFile(pData, ulSize))))
//!     return 0;
//!
//! // do some legacy stuff
//! // therefore retrieve a legacy object
//! ChaspLegacy legacy = hasp.getLegacy();
//! if (!legacy.isValid())
//!     return 0;
//!
//! if (!HASP_SUCCEEDED(legacy.setRtc(ChaspTime(1, 1, 200, 0, 0, 0))))
//!     return 0;
//!
//! // advanced stuff
//! if (!HASP_SUCCEEDED(hasp.logout()))
//!     return 0;
//!
//! // not working anymore
//! if (file.isLoggedIn() || legacy.isLoggedIn())
//!     return 0;
//!
//! // no need to call special clean up - 
//! // resources are freed when last instance goes out of scope
//!
//! \endcode
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
// Construction/Destruction
////////////////////////////////////////////////////////////////////

Chasp::Chasp()
{
}

////////////////////////////////////////////////////////////////////
//! Copy constructor.
////////////////////////////////////////////////////////////////////
Chasp::Chasp(const Chasp& other)
    : ChaspBase(other)
{
}

////////////////////////////////////////////////////////////////////
//! Copy constructor.
////////////////////////////////////////////////////////////////////
Chasp::Chasp(const ChaspBase& other)
    : ChaspBase(other)
{
}

////////////////////////////////////////////////////////////////////
//! Constructs a new object and links it to a private key.
////////////////////////////////////////////////////////////////////
Chasp::Chasp(const ChaspFeature& feature)
    : ChaspBase(feature.feature())
{
}

////////////////////////////////////////////////////////////////////
// Implementation
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
// Assignment operator
////////////////////////////////////////////////////////////////////
Chasp& Chasp::operator=(const ChaspBase& other)
{
    dynamic_cast<ChaspBase&>(*this) = other;
    return *this;
}

////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////
Chasp& Chasp::operator=(const Chasp& other)
{
    dynamic_cast<ChaspBase&>(*this) = other;
    return *this;
}

////////////////////////////////////////////////////////////////////
//! Decrypts the specified data using an AES key.
//!
//! \param pData                Pointer to the first byte to be
//!                             decrypted.
//! \param ulSize               The size of the buffer to be
//!                             decrypted.
//!
//! \return                     A \a haspStatus status code.
//!
//! \sa                         ChaspImpl::decrypt
////////////////////////////////////////////////////////////////////
haspStatus Chasp::decrypt(unsigned char* pData, 
                          hasp_size_t ulSize) const
{
    HASP_PROLOGUE(m_handle);
    return HASP_KEYPTR->decrypt(pData, ulSize);
}

////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////
haspStatus Chasp::decrypt(const char*) const
{
    return HASP_INVALID_PARAMETER;
}

////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////
haspStatus Chasp::decrypt(std::string& data) const
{
    std::vector<unsigned char> vector;
    ChaspBase64::decode(data, vector);

    haspStatus nStatus = decrypt(&vector[0], 
                                 static_cast<hasp_size_t>(vector.size()));
    if (HASP_STATUS_OK != nStatus)
        return nStatus;

    data.resize(0);
    for (std::vector<unsigned char>::const_iterator it = vector.begin();
         (vector.end() != it) && (0 != *it);
         it++)
        data += *it;

    return HASP_STATUS_OK;
}

////////////////////////////////////////////////////////////////////
//! Encrypts the specified data using an AES key.
//!
//! \param pData                Pointer to the first byte to be
//!                             encrypted.
//! \param ulSize               The size of the buffer to be
//!                             encrypted.
//!
//! \return                     A \a haspStatus status code.
//!
//! \sa                         ChaspImpl::encrypt
////////////////////////////////////////////////////////////////////
haspStatus Chasp::encrypt(unsigned char* pData, 
                          hasp_size_t ulSize) const
{
    HASP_PROLOGUE(m_handle);
    return HASP_KEYPTR->encrypt(pData, ulSize);
}

////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////
haspStatus Chasp::encrypt(const char*) const
{
    return HASP_INVALID_PARAMETER;
}

////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////
haspStatus Chasp::encrypt(std::string& data) const
{
    std::vector<unsigned char> vector;
    std::copy(data.begin(), data.end(), std::back_inserter(vector));
    if (HASP_MIN_BLOCK_SIZE > vector.size())
        vector.resize(HASP_MIN_BLOCK_SIZE, 0);

    haspStatus nStatus= encrypt(&vector[0], 
                                static_cast<hasp_size_t>(vector.size()));
    if (HASP_STATUS_OK != nStatus)
        return nStatus;

    ChaspBase64::encode(vector, data);
    return HASP_STATUS_OK;
}

////////////////////////////////////////////////////////////////////
//! Returns the feature identifier for the this Chasp object.
//!
//! \return                     A ChaspFeature instance.
//!
//! \sa                         ChaspImpl::feature
////////////////////////////////////////////////////////////////////
ChaspFeature Chasp::feature() const
{
    DIAG_ASSERT(isValid());

    // lock access
    ChaspMap map;
    ChaspImpl* pKey = map.getKey(m_handle);

    DIAG_ASSERT(NULL != pKey);
    if (NULL != pKey)
        return ChaspFeature(pKey->feature());
    
    return ChaspFeature::defaultFeature();
}

////////////////////////////////////////////////////////////////////
//! Returns an instance of the ChaspFile class. The instance
//! references the same key as the instance of the Chasp class.
//!
//! \return                     An instance of the ChaspFile class.
////////////////////////////////////////////////////////////////////
ChaspFile Chasp::getFile() const
{
    return getFile(ChaspFile::fileMain);
}

////////////////////////////////////////////////////////////////////
//! Returns an instance of the ChaspFile class. The instance
//! references the same key as the instance of the Chasp class.
//!
//! \return                     An instance of the ChaspFile class.
////////////////////////////////////////////////////////////////////
ChaspFile Chasp::getFile(hasp_fileid_t fileId) const
{
    // lock access
    ChaspMap map;

    DIAG_ASSERT(isValid());

    ChaspFile file(fileId, (isValid() ? *this : Chasp()));
    DIAG_ASSERT(isValid() ? file.isValid() : true);

    return file;
}

////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////
haspStatus Chasp::getInfo(const char* pszQuery, 
                          const char* pszFormat, 
                          hasp_vendor_code_t vendorCode, 
                          std::string& info)
{
    info.resize(0);
    ChaspInfo _info;

    haspStatus status = ChaspImpl::getInfo(pszQuery, 
                                           pszFormat, 
                                           vendorCode,
                                           _info);

    if (HASP_SUCCEEDED(status) && (NULL != _info.getInfo()))
        info = _info;

    return status; 
}

////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////
haspStatus Chasp::getInfo(const char* pszQuery, 
                          const std::string& format, 
                          hasp_vendor_code_t vendorCode, 
                          std::string& info)
{
    return Chasp::getInfo(pszQuery,
                          format.c_str(),
                          vendorCode,
                          info);
}

////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////
haspStatus Chasp::getInfo(const std::string& query, 
                          const char* pszFormat, 
                          hasp_vendor_code_t vendorCode, 
                          std::string& info)
{
    return Chasp::getInfo(query.c_str(),
                          pszFormat,
                          vendorCode,
                          info);
}

////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////
haspStatus Chasp::getInfo(const std::string& query, 
                          const std::string& format, 
                          hasp_vendor_code_t vendorCode, 
                          std::string& info)
{
    return Chasp::getInfo(query.c_str(),
                          format.c_str(),
                          vendorCode,
                          info);
}

////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////
haspStatus Chasp::detach(const std::string& action, const std::string& scope, hasp_vendor_code_t vendorCode, 
						 const std::string& recipient, std::string& v2c)
{
	return Chasp::detach(action.c_str(),
						 scope.c_str(),
						 vendorCode,
						 recipient.c_str(),
						 v2c);
}

////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////
haspStatus Chasp::detach(const char* pszAction, const char* pszScope, hasp_vendor_code_t vendorCode, 
						 const char* pszRecipient, std::string& v2c)
{
	v2c.resize(0);
	ChaspInfo _info;

	haspStatus status = ChaspImpl::detach(pszAction, 
										  pszScope, 
										  vendorCode,
										  pszRecipient,
										  _info);

	if (HASP_SUCCEEDED(status) && (NULL != _info.getInfo()))
		v2c = _info;

	return status; 
}


////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////
haspStatus Chasp::transfer(const std::string& action, const std::string& scope, hasp_vendor_code_t vendorCode, 
						 const std::string& recipient, std::string& v2c)
{
	return Chasp::transfer(action.c_str(),
						 scope.c_str(),
						 vendorCode,
						 recipient.c_str(),
						 v2c);
}

////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////
haspStatus Chasp::transfer(const char* pszAction, const char* pszScope, hasp_vendor_code_t vendorCode, 
						 const char* pszRecipient, std::string& v2c)
{
	v2c.resize(0);
	ChaspInfo _info;

	haspStatus status = ChaspImpl::transfer(pszAction, 
										  pszScope, 
										  vendorCode,
										  pszRecipient,
										  _info);

	if (HASP_SUCCEEDED(status) && (NULL != _info.getInfo()))
		v2c = _info;

	return status; 
}

////////////////////////////////////////////////////////////////////
//! Returns an instance of the ChaspLegacy class. The instance
//! references the same key as as the instance of the Chasp class.
//!
//! \return                     An instance of the ChaspLegacy 
//!                             class.
//!
//! \note                       The returned instance is only valid
//!                             if the object has a Hasp4 feature.
////////////////////////////////////////////////////////////////////
ChaspLegacy Chasp::getLegacy() const
{
    // lock access
    ChaspMap map;

    DIAG_ASSERT(isValid());

    // only instantiate with *this when
    // valid and feature is a Hasp4 feature.
    ChaspLegacy legacy((hasLegacy() ? *this : Chasp()));
    DIAG_ASSERT(hasLegacy() ? legacy.isValid() : true);

    return legacy;
}

////////////////////////////////////////////////////////////////////
//! Returns the date and time of the Hasp's real time clock.
//!
//! \param time                 A reference to a ChaspTime 
//!                             receiving the Hasp's time.
//!
//! \return                     A \a haspStatus status code.
//!
//! \sa                         Chaspimpl::getRtc
////////////////////////////////////////////////////////////////////
haspStatus Chasp::getRtc(ChaspTime& time) const
{
    HASP_PROLOGUE(m_handle);
    
    hasp_time_t rtc = 0;
    haspStatus status = HASP_KEYPTR->getRtc(rtc);

    if (HASP_SUCCEEDED(status))
    {
        ChaspTime _time(rtc);
        time = _time;
    }

    return status;
}

////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////
haspStatus Chasp::getSessionInfo(const char* pszFormat, 
                                 ChaspInfo& info) const
{
    HASP_PROLOGUE(m_handle);
    return HASP_KEYPTR->getSessionInfo(pszFormat, info);
}

////////////////////////////////////////////////////////////////////
//! Returns information about the session.
////////////////////////////////////////////////////////////////////
haspStatus Chasp::getSessionInfo(const std::string& format, 
                                 std::string& info) const
{ 
    info.resize(0);
    ChaspInfo _info;
    
    haspStatus status = getSessionInfo(format.c_str(), _info);
    if (HASP_SUCCEEDED(status) && (NULL != _info.getInfo()))
        info = _info;
    
    return status; 
}

////////////////////////////////////////////////////////////////////
//! Returns the hasp version
////////////////////////////////////////////////////////////////////
haspStatus Chasp::getVersion(hasp_vendor_code_t vendorCode,
                             ChaspVersion& version)
{
    return ChaspImpl::getVersion(vendorCode, version);
}

////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////
bool Chasp::hasLegacy() const
{
    DIAG_ASSERT(isValid());

    ChaspFeature _feature = feature();
    return isValid() && _feature.isProgNum();
}

////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////
std::string Chasp::keyInfo()
{ 
    return std::string(HASP_KEYINFO); 
}

////////////////////////////////////////////////////////////////////
//! Performs a login to HASP protection key.
//!
//! \param  scope               scope parameter
//! \param  vendorCode          Pointer to the vendor code.
//!
//! \return                     A \a haspStatus status code.
//!
//! \sa                         ChaspImpl::loginScope
////////////////////////////////////////////////////////////////////
haspStatus Chasp::login(hasp_vendor_code_t vendorCode, 
                        const std::string& scope)
{
    return login(vendorCode, scope.c_str());
}

////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////
haspStatus Chasp::login(hasp_vendor_code_t vendorCode, 
                        const char* pszScope /* = 0*/)
{
    HASP_PROLOGUE(m_handle);
    return HASP_KEYPTR->login(vendorCode, pszScope);
}

////////////////////////////////////////////////////////////////////
//! Performs a logout from HASP protection key.
//!
//! \return                     A \a haspStatus status code.
//!
//! \sa                         ChaspImpl::logout
////////////////////////////////////////////////////////////////////
haspStatus Chasp::logout()
{
    HASP_PROLOGUE(m_handle);
    haspStatus status = HASP_KEYPTR->logout();

    // keep the logout counter current
    synchronize();
    return status;
}

////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////
std::string Chasp::sessionInfo()
{ 
    return std::string(HASP_SESSIONINFO); 
}

////////////////////////////////////////////////////////////////////
//! Synchronizes the logout counter with the private class' logout
//! counter. Usually called by isValid
////////////////////////////////////////////////////////////////////
void Chasp::synchronize() const
{
    ChaspMap map;
    ChaspImpl* pKey = map.getKey(m_handle);

    DIAG_ASSERT(NULL != pKey);
    if (NULL != pKey)
    {
        // keep the logout counter current
        ChaspHandle& handle = const_cast<ChaspHandle&>(m_handle);
        handle.m_ulAltered = pKey->altered();
    }
}

////////////////////////////////////////////////////////////////////
//! Returns the login handle as a human readable string.
//!
//! \sa                         ChaspImpl::toString
////////////////////////////////////////////////////////////////////
std::string Chasp::toString() const
{
    ChaspMap map;

    DIAG_ASSERT(isValid());
    if (!isValid())
        return std::string();

    ChaspImpl* pKey = map.getKey(m_handle);
    DIAG_ASSERT(NULL != pKey);

    return (NULL == pKey) ? std::string() : pKey->toString();
}

////////////////////////////////////////////////////////////////////
//! Performs HASP protection key update operation.
////////////////////////////////////////////////////////////////////
haspStatus Chasp::update(const char* pszUpdate, 
                         ChaspInfo& acknowledge)
{
    return ChaspImpl::update(pszUpdate, acknowledge);
}

////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////
haspStatus Chasp::update(const char* pszUpdate, 
                         std::string& acknowledge)
{
    acknowledge.resize(0);
    ChaspInfo ack;
    
    haspStatus status = Chasp::update(pszUpdate, ack);
    if (HASP_SUCCEEDED(status) && (NULL != ack.getInfo()))
        acknowledge = ack;

    return status; 
}

////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////
haspStatus Chasp::update(const std::string& update, 
                         std::string& acknowledge)
{ 
    return Chasp::update(update.c_str(), acknowledge);
}

////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////
std::string Chasp::updateInfo()
{ 
    return std::string(HASP_UPDATEINFO); 
}

std::string Chasp::recipientInfo()
{ 
    return std::string(HASP_RECIPIENT); 
}
