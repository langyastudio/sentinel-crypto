////////////////////////////////////////////////////////////////////
// Copyright (C) 2011, SafeNet, Inc. All rights reserved.
//
// HASP(R) is a registered trademark of SafeNet, Inc. 
//
//
// $Id: hasplegacy.cpp,v 1.19 2011-05-02 03:12:07 nsingh1 Exp $
////////////////////////////////////////////////////////////////////
#include "hasp_api_cpp_.h"


////////////////////////////////////////////////////////////////////
//! \class ChaspLegacy hasp_api_cpp.h
//! \brief Class permitting access to the Hasp4 legacy functions.
//!
//! 
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
// Construction/Destruction
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
//! Intializes the object.
////////////////////////////////////////////////////////////////////
ChaspLegacy::ChaspLegacy()
{
}

////////////////////////////////////////////////////////////////////
//! Copy constructor intializing object with other's private key.
////////////////////////////////////////////////////////////////////
ChaspLegacy::ChaspLegacy(const Chasp& other)
    : ChaspBase(other)
{
    if (!other.hasLegacy())
        DIAG_VERIFY(release());
}

////////////////////////////////////////////////////////////////////
//! Copy constructor intializing object with other's private key.
////////////////////////////////////////////////////////////////////
ChaspLegacy::ChaspLegacy(const ChaspLegacy& other)
    : ChaspBase(other)
{
}

////////////////////////////////////////////////////////////////////
// Implementation
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
//! Assignment operator. Initializes the object with other's 
//! private key after releasing its private key object.
////////////////////////////////////////////////////////////////////

ChaspLegacy& ChaspLegacy::operator=(const Chasp& other)
{
    if ((*this != other) && other.hasLegacy())
    {
        dynamic_cast<ChaspBase&>(*this) = 
            dynamic_cast<const ChaspBase&>(other);
    }

    return *this;
}

ChaspLegacy& ChaspLegacy::operator=(const ChaspLegacy& other)
{
    dynamic_cast<ChaspBase&>(*this) = 
        dynamic_cast<const ChaspBase&>(other);

    return *this;
}

////////////////////////////////////////////////////////////////////
//! Decrypts the specified data using the Hasp4 algorithm.
//!
//! \param pData                Pointer to the first byte to be
//!                             decrypted.
//! \param ulSize               The size of the buffer to be
//!                             decrypted.
//!
//! \return                     A \a haspStatus status code.
//!
//! \sa                         ChaspImpl::legacyDecrypt
////////////////////////////////////////////////////////////////////
haspStatus ChaspLegacy::decrypt(unsigned char* pData, 
                                hasp_size_t ulSize) const
{
    HASP_PROLOGUE(m_handle);
    return HASP_KEYPTR->legacyDecrypt(pData, ulSize);
}

////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////
haspStatus ChaspLegacy::decrypt(const char*) const
{
    return HASP_INVALID_PARAMETER;
}

////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////
haspStatus ChaspLegacy::decrypt(std::string& data) const
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
//! Encrypts the specified data using the Hasp4 algorithm.
//!
//! \param pData                Pointer to the first byte to be
//!                             encrypted.
//! \param ulSize               The size of the buffer to be
//!                             encrypted.
//!
//! \return                     A \a haspStatus status code.
//!
//! \sa                         ChaspImpl::legacyEncrypt
////////////////////////////////////////////////////////////////////
haspStatus ChaspLegacy::encrypt(unsigned char* pData, 
                                hasp_size_t ulSize) const
{
    HASP_PROLOGUE(m_handle);
    return HASP_KEYPTR->legacyEncrypt(pData, ulSize);
}

////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////
haspStatus ChaspLegacy::encrypt(const char*) const
{
    return HASP_INVALID_PARAMETER;
}

////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////
haspStatus ChaspLegacy::encrypt(std::string& data) const
{
    std::vector<unsigned char> vector;
    std::copy(data.begin(), data.end(), std::back_inserter(vector));
    if (HASP_MIN_BLOCK_SIZE_LEGACY > vector.size())
        vector.resize(HASP_MIN_BLOCK_SIZE, 0);

    haspStatus nStatus= encrypt(&vector[0], 
                                static_cast<hasp_size_t>(vector.size()));
    if (HASP_STATUS_OK != nStatus)
        return nStatus;

    ChaspBase64::encode(vector, data);
    return HASP_STATUS_OK;
}

////////////////////////////////////////////////////////////////////
//! Sets the idle time.
//!
//! \param nIdleTime            The idle time to be set.
//!
//! \return                     A \a haspStatus status code.
//!
//! \sa                         ChaspImpl::setIdleTime
////////////////////////////////////////////////////////////////////
haspStatus ChaspLegacy::setIdleTime(hasp_u16_t nIdleTime) const
{
    HASP_PROLOGUE(m_handle);
    return HASP_KEYPTR->setIdleTime(nIdleTime);
}

////////////////////////////////////////////////////////////////////
//! Sets the date and time of the real time clock.
//!
//! \param time                 The date and time to be set.
//!
//! \return                     A \a haspStatus status code.
//!
//! \sa                         ChaspImpl::setRtc
////////////////////////////////////////////////////////////////////
haspStatus ChaspLegacy::setRtc(const ChaspTime& time) const
{
    HASP_PROLOGUE(m_handle);
    return HASP_KEYPTR->legacySetRtc(time);
}

////////////////////////////////////////////////////////////////////
//! Returns the login handle as a human readable string.
//!
//! \sa                         ChaspImpl::toString
////////////////////////////////////////////////////////////////////
std::string ChaspLegacy::toString() const
{
    ChaspMap map;

    DIAG_ASSERT(isValid());
    if (!isValid())
        return std::string();

    ChaspImpl* pKey = map.getKey(m_handle);
    DIAG_ASSERT(NULL != pKey);

    return (NULL == pKey) ? std::string() : pKey->toString();
}
