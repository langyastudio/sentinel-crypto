////////////////////////////////////////////////////////////////////
// Copyright (C) 2011, SafeNet, Inc. All rights reserved.
//
// HASP(R) is a registered trademark of SafeNet, Inc. 
//
//
// $Id: haspbase.cpp,v 1.19 2011-05-02 03:12:07 nsingh1 Exp $
////////////////////////////////////////////////////////////////////
#include "hasp_api_cpp_.h"

////////////////////////////////////////////////////////////////////
//! \class ChaspBase hasp_api_cpp.h
//! \brief Pure virtual base class for all other public key classes.
//!
//! The ChaspBase class implements the mechanism for accessing a
//! private Hasp class providing the methods for accessing a
//! Sentinel Hasp key.
//! ChaspBase and all derived classes are only wrappers and pass
//! their calls to this private object.
//! To maintain thread safety this is done via a mapping class,
//! which provides methods for locking the access to the private key 
//! object. The private class itself hold a reference counter and
//! only goes out of scope when the last reference to it is gone
//! out of scope too.
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
// Construction/Destruction
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
//! Constructs an empty object to which an existing key can
//! be attached later.
//! \sa                         operator=
//! \sa                         isValid
////////////////////////////////////////////////////////////////////
ChaspBase::ChaspBase()
{
    // Ensure that the ChaspMap singleton is constructed before any other possible static ChaspBase object
    // And if it's constructed before, it's also destroyed after.
    ChaspMap::map(); 
}

////////////////////////////////////////////////////////////////////
//! Copy constructor. Adds a reference to \a other's private
//! key.
//!
//! \sa                         isValid
////////////////////////////////////////////////////////////////////
ChaspBase::ChaspBase(const ChaspBase& other)
{
    *this = other;

    // Ensure that the ChaspMap singleton is constructed before any other possible static ChaspBase object
    // And if it's constructed before, it's also destroyed after.
    ChaspMap::map();     
}

////////////////////////////////////////////////////////////////////
//! Constructs a new object and links it to a private key.
////////////////////////////////////////////////////////////////////
ChaspBase::ChaspBase(hasp_feature_t feature)
{
    DIAG_VERIFY(construct(feature));
    DIAG_ASSERT(isValid());
    
    // Ensure that the ChaspMap singleton is constructed before any other possible static ChaspBase object
    // And if it's constructed before, it's also destroyed after.
    ChaspMap::map();     
}

ChaspBase::~ChaspBase()
{
    DIAG_VERIFY(release());
}

////////////////////////////////////////////////////////////////////
// Implementation
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
//! Overloaded assignment operator.
////////////////////////////////////////////////////////////////////
ChaspBase& ChaspBase::operator=(const ChaspBase& other)
{
    if (this != &other)
        addRef(other);

    return *this;
}

////////////////////////////////////////////////////////////////////
//! Overloaded equal operator.
//!
//! \return                     \a true if the object points to the
//!                             same private key object as \a other,
//!                             \a false otherwise.
////////////////////////////////////////////////////////////////////
bool ChaspBase::operator==(const ChaspBase& other) const
{
    // lock access
    ChaspMap map;

    return (this == &other) ||
           (isValid() && 
            other.isValid() &&
            (other.handle() == m_handle));
}

////////////////////////////////////////////////////////////////////
//! Overloaded not equal operator.
//!
//! \sa                         operator==
////////////////////////////////////////////////////////////////////
bool ChaspBase::operator!=(const ChaspBase& other) const
{
    return !(*this == other);
}

////////////////////////////////////////////////////////////////////
//! Adds a reference to the private key via the map and links 
//! this object to the private key referenced by \a other.
//!
//! \param other                Reference to another ChaspBase.
//!
//! \return                     \a true on success, \a false
//!                             otherwise.
//!
//! \sa                         ChaspBase::construct
//!                             ChaspBase::release
////////////////////////////////////////////////////////////////////
bool ChaspBase::addRef(const ChaspBase& other)
{
    // lock access
    ChaspMap map;

    // release attached key if present
    if (!release())
    {
        DIAG_ASSERT(!"Release failed");
        return false;
    }

    DIAG_ASSERT(m_handle.isNull());
    m_handle.clear();

    if (!other.isValid())
    {
        DIAG_ASSERT(!"Other not valid");
        return false;
    }

    ChaspHandle handle = other.handle();

    // add a reference
    if (!map.addRef(handle))
    {
        DIAG_ASSERT(!"addRef failed");
        return false;
    }

    m_handle = handle;
    return true;
}

////////////////////////////////////////////////////////////////////
//! Creates a new private key and initializes it with \a feature.
//!
//! \return                     \a true on success, \a false
//!                             otherwhise.
//!
//! \sa                         ChaspBase::addRef
//!                             ChaspBase::release
////////////////////////////////////////////////////////////////////
bool ChaspBase::construct(hasp_feature_t feature)
{
    if (!release())
    {
        DIAG_ASSERT(!"release failed");
        return false;
    }

    return ChaspMap::createKey(feature, m_handle);
}

////////////////////////////////////////////////////////////////////
//! Detaches the private key from the object by releasing it.
//! Use this method to detach the object from a key.
//!
//! \return                     \a true on success, \a false 
//!                             otherwhise.
////////////////////////////////////////////////////////////////////
bool ChaspBase::dispose()
{
    if (!isValid())
        return true;

    bool bResult = release();
    DIAG_ASSERT(m_handle.isNull());

    return bResult;
}

////////////////////////////////////////////////////////////////////
//! Returns the handle.
////////////////////////////////////////////////////////////////////
ChaspHandle ChaspBase::handle() const
{
    return m_handle;
}

////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////
hasp_u32_t ChaspBase::hashCode() const
{
    ChaspMap map;

    if (!isValid()) 
        return static_cast<hasp_u32_t>(-1);
    
    ChaspImpl* pKey = map.getKey(m_handle);
    DIAG_ASSERT(NULL != pKey);
    
    return (NULL == pKey) ?  
            static_cast<hasp_u32_t>(-1) :
            pKey->handle();
}

////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////
bool ChaspBase::isKindOf(const std::type_info& info) const
{
    return (0 != (typeid(*this) == info));
}

////////////////////////////////////////////////////////////////////
//! Used to determine if the object is logged in a Sentinel Hasp key.
//!
//! \return                     \a true if logged in, \a false
//!                             otherwise.
////////////////////////////////////////////////////////////////////
bool ChaspBase::isLoggedIn() const
{
    ChaspMap map;

    if (!isValid())
        return false;
    
    ChaspImpl* pKey = map.getKey(m_handle);
    DIAG_ASSERT(NULL != pKey);
    
    return (NULL != pKey) && pKey->isLoggedIn();
}

////////////////////////////////////////////////////////////////////
//! Checks whether the object is valid, means it is attached to
//! to a private key.
//!
//! \return                     \a true if a Sentinel Hasp key can be
//!                             accessed via this object, \a false
//!                             otherwise.
////////////////////////////////////////////////////////////////////
bool ChaspBase::isValid() const
{
    if (m_handle.isNull())
        return false;

    ChaspMap map;
    ChaspImpl* pKey = map.getKey(m_handle);

    if (NULL == pKey)
        return false;

    synchronize();

    return !pKey->isAltered(m_handle);
}

////////////////////////////////////////////////////////////////////
//! Releases the private key this instance points to and 
//! invalidates the object.
//!
//! \return                     \a true on success, \a false
//!                             otherwhise.
////////////////////////////////////////////////////////////////////
bool ChaspBase::release()
{
    return (m_handle.isNull() || ChaspMap::release(m_handle));
}

////////////////////////////////////////////////////////////////////
//! Called before any call to the private class' methods
//! for synchronizing states etc.
//!
//! \note                       This implementation does nothing
//!                             by default. Derived classes must
//!                             implement it to stay in sync. with
//!                             the private class when needed.
////////////////////////////////////////////////////////////////////
void ChaspBase::synchronize() const
{
    // this function intentionally left blank
}
