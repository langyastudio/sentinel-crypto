////////////////////////////////////////////////////////////////////
// Copyright (C) 2010, SafeNet, Inc. All rights reserved.
//
// HASP(R) is a registered trademark of SafeNet, Inc. 
//
//
// $Id: haspmap.cpp,v 1.16 2010-07-19 03:03:04 pwang Exp $
////////////////////////////////////////////////////////////////////
#include "hasp_api_cpp_.h"


////////////////////////////////////////////////////////////////////
// struct ChaspMapEntry
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
// Construction/Destruction
////////////////////////////////////////////////////////////////////

ChaspMapEntry::ChaspMapEntry()
    : m_ulCount(0),
      m_pKey(NULL)
{
}

ChaspMapEntry::~ChaspMapEntry()
{
}


////////////////////////////////////////////////////////////////////
// class ChaspMapImpl
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
// Initialization
////////////////////////////////////////////////////////////////////

const hasp_u32_t ChaspMapImpl::m_ulInitCapacity = 0xFFFF;

////////////////////////////////////////////////////////////////////
// Construction/Destruction
////////////////////////////////////////////////////////////////////
ChaspMapImpl::ChaspMapImpl()
{
    DIAG_TRACE("ChaspMapImpl::ChaspMapImpl\n");

    DIAG_ASSERT(m_lock.isInit());

    // reserve a certain amount by default.
    m_map.reserve(m_ulInitCapacity);

    // zero index is reserved for invalid handles
    // so store a dummy entry there.
    ChaspMapEntry dummy;
    m_map.push_back(dummy);
}

ChaspMapImpl::~ChaspMapImpl()
{
    DIAG_TRACE("ChaspMapImpl::~ChaspMapImpl\n");

    for (std::vector<ChaspMapEntry>::iterator it = m_map.begin();
         it != m_map.end(); 
         it++)
    {
        DIAG_ASSERT(it->m_pKey == NULL);
        while (it->m_pKey)
            it->m_pKey->release();
    }
}

////////////////////////////////////////////////////////////////////
// Implementation
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////
bool ChaspMapImpl::addRef(ChaspHandle& handle) const
{
    DIAG_ASSERT(!handle.isNull());

    if (handle.isNull() || !lockMap())
        return false;

    bool bResult = false;
    if (m_map.size() > handle.m_ulIndex)
    {
        const ChaspMapEntry& entry = m_map[handle.m_ulIndex];
        if ((entry.m_ulCount == handle.m_ulCount) &&
            (NULL != entry.m_pKey))
            bResult = entry.m_pKey->addRef(handle);
    }

    DIAG_VERIFY(unlockMap());
    return bResult;
}

////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////
bool ChaspMapImpl::createKey(hasp_u32_t feature, 
                             ChaspHandle& handle)
{
    handle.clear();
    DIAG_ASSERT(handle.isNull());

    if (!lockMap())
        return false;

    ChaspImpl* pKey = DIAG_NEW ChaspImpl(feature, handle);

    for (hasp_u32_t ulIndex = 1; m_map.size() > ulIndex; ulIndex++)
    {
        if (NULL == m_map[ulIndex].m_pKey)  
        {
            handle.m_ulIndex = ulIndex;
            handle.m_ulCount = m_map[ulIndex].m_ulCount;
            break;
        }
    }

    if (0 == handle.m_ulIndex)
    {
        m_map.push_back(ChaspMapEntry());
        handle.m_ulIndex = static_cast<hasp_u32_t>(m_map.size() - 1);
    }

    m_map[handle.m_ulIndex].m_pKey = pKey;
    
    DIAG_VERIFY(unlockMap());   
    return true;
}

////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////
hasp_u32_t ChaspMapImpl::findKey(const ChaspImpl* pKey)
{
    hasp_u32_t ulFound = 0;

    DIAG_ASSERT(NULL != pKey);
    if ((NULL == pKey) || !lockMap())
        return ulFound;

    for (hasp_u32_t ulIndex = 1; m_map.size() > ulIndex; ulIndex++)
    {
        if (m_map[ulIndex].m_pKey == pKey)
        {
            ulFound = ulIndex;
            break;
        }
    }
    
    DIAG_VERIFY(unlockMap());
    return ulFound;
}

////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////
ChaspImpl* ChaspMapImpl::getKey(const ChaspHandle& handle) const
{
    DIAG_ASSERT(!handle.isNull());

    if (handle.isNull() || (m_map.size() <= handle.m_ulIndex))
        return NULL;

    const ChaspMapEntry& entry = m_map[handle.m_ulIndex];
    return (entry.m_ulCount == handle.m_ulCount) ? 
                entry.m_pKey : NULL;
}

////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////
bool ChaspMapImpl::lockMap() const
{
    ChaspMapImpl* pMap = const_cast<ChaspMapImpl*>(this);
    return pMap->m_lock.lock();
}

////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////
bool ChaspMapImpl::release(ChaspHandle& handle) const
{
    DIAG_ASSERT(!handle.isNull());

    if (handle.isNull() || !lockMap())
        return false;

    bool bResult = false;

    DIAG_ASSERT(m_map.size() > handle.m_ulIndex);
    if (m_map.size() > handle.m_ulIndex)
    {
        const ChaspMapEntry& entry = m_map[handle.m_ulIndex];

        if (entry.m_ulCount == handle.m_ulCount)
        {
            DIAG_ASSERT(NULL != entry.m_pKey);
            bResult = (NULL == entry.m_pKey) ? 
                        true : entry.m_pKey->release();
        }
    }
    
    if (bResult)
        handle.clear();

    DIAG_VERIFY(unlockMap());
    return bResult;
}

////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////
bool ChaspMapImpl::removeKey(const ChaspImpl* pKey)
{
    if (NULL == pKey)
        return true;

    for (std::vector<ChaspMapEntry>::iterator it = m_map.begin();
         it != m_map.end(); 
         it++)
    {
        if (it->m_pKey == pKey)
        {
            it->m_ulCount++;
            it->m_pKey = NULL;
            return true;
        }
    }

    return false;
}

////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////
bool ChaspMapImpl::unlockMap() const
{
    ChaspMapImpl* pMap = const_cast<ChaspMapImpl*>(this);
    return pMap->m_lock.unlock();
}


////////////////////////////////////////////////////////////////////
// class ChaspMap
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
// Initialization
////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////
// Construction/Destruction
////////////////////////////////////////////////////////////////////

ChaspMap::ChaspMap()
    : m_bLocked(false)
{
    m_bLocked = map().lockMap();
    DIAG_ASSERT(m_bLocked);
}

ChaspMap::~ChaspMap()
{
    if (m_bLocked)
        DIAG_VERIFY(map().unlockMap());
}

////////////////////////////////////////////////////////////////////
// Implementation
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////
bool ChaspMap::addRef(ChaspHandle& handle)
{
    return map().addRef(handle);
}

////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////
bool ChaspMap::createKey(hasp_u32_t feature, ChaspHandle& handle)
{
    return map().createKey(feature, handle);
}

////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////
hasp_u32_t ChaspMap::findKey(const ChaspImpl* pKey)
{
    return map().findKey(pKey);
}

////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////
ChaspImpl* ChaspMap::getKey(const ChaspHandle& handle) const
{
    DIAG_ASSERT(m_bLocked);
    return m_bLocked ? map().getKey(handle) : NULL;
}

////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////
ChaspMapImpl& ChaspMap::map()
{
    static ChaspMapImpl _theMap; // It's a Meyer's singleton. The constructor is called at the first call.
    return _theMap;
}

////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////
bool ChaspMap::release(ChaspHandle& handle)
{
    return map().release(handle);
}

////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////
bool ChaspMap::removeKey(const ChaspImpl* pKey)
{
    return map().removeKey(pKey);
}
