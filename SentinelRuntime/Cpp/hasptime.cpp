////////////////////////////////////////////////////////////////////
// Copyright (C) 2010, SafeNet, Inc. All rights reserved.
//
// HASP(R) is a registered trademark of SafeNet, Inc. 
//
//
// $Id: hasptime.cpp,v 1.12 2010-06-08 07:23:41 pwang Exp $
////////////////////////////////////////////////////////////////////
#include "hasp_api_cpp_.h"
#include <string.h>

////////////////////////////////////////////////////////////////////
//! \struct ChaspTimeHelper
//! \brief Helper class for managing the time via the low level API.
////////////////////////////////////////////////////////////////////
struct ChaspTimeHelper
{
// Construction/Destruction
public:
    ChaspTimeHelper();
    ~ChaspTimeHelper();

// Attributes
public:
    unsigned int                        m_nYear;
    unsigned int                        m_nMonth;
    unsigned int                        m_nDay;
    unsigned int                        m_nHour;
    unsigned int                        m_nMinute;
    unsigned int                        m_nSecond;

// Implementation
public:
    hasp_time_t toTime() const;
    bool toTime(const hasp_time_t time);
};


////////////////////////////////////////////////////////////////////
// struct ChaspTimeHelper Implementation
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
// Construction/Destruction
////////////////////////////////////////////////////////////////////

ChaspTimeHelper::ChaspTimeHelper()
{
    memset(this, 0x00, sizeof(*this));
}

ChaspTimeHelper::~ChaspTimeHelper()
{
    memset(this, 0x00, sizeof(*this));
}

////////////////////////////////////////////////////////////////////
// Implementation
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
//! Converts the stored time to the \a hasp_time_t type.
////////////////////////////////////////////////////////////////////
hasp_time_t ChaspTimeHelper::toTime() const
{
    hasp_time_t time = 0;

    if (!HASP_SUCCEEDED(hasp_datetime_to_hasptime(m_nDay,
                                                  m_nMonth,
                                                  m_nYear,
                                                  m_nHour,
                                                  m_nMinute,
                                                  m_nSecond,
                                                  &time)))
        time = 0;

    return time;
}

////////////////////////////////////////////////////////////////////
//! Converts the given time \a time into the internal structure.
////////////////////////////////////////////////////////////////////
bool ChaspTimeHelper::toTime(hasp_time_t time)
{
    if (!HASP_SUCCEEDED(hasp_hasptime_to_datetime(time,
                                                  &m_nDay,
                                                  &m_nMonth,
                                                  &m_nYear,
                                                  &m_nHour,
                                                  &m_nMinute,
                                                  &m_nSecond)))
    {
        memset(this, 0x00, sizeof(*this));
        return false;
    }

    return true;
}


////////////////////////////////////////////////////////////////////
//! \struct ChaspTime haspcpp_.h
//! \brief Class encapsulating HASP protection key date and time.
//!
//! The ChaspTime class encapsultes HASP protection key time and provides 
//! methods for converting HASP protection key data and time.
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
// Construction/Destruction
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
//! Initializes the object with the passed \a time.
////////////////////////////////////////////////////////////////////
ChaspTime::ChaspTime(hasp_time_t time /* = 0 */)
    : m_time(time)
{
}

////////////////////////////////////////////////////////////////////
//! Initializes the object with the passed data and time.
////////////////////////////////////////////////////////////////////
ChaspTime::ChaspTime(unsigned int nYear, unsigned int nMonth, 
                     unsigned int nDay, unsigned int nHour, 
                     unsigned int nMinute, unsigned int nSecond)
    : m_time(0)
{
    ChaspTimeHelper helper;
    helper.m_nYear = nYear;
    helper.m_nMonth = nMonth;
    helper.m_nDay = nDay;
    helper.m_nHour = nHour;
    helper.m_nMinute = nMinute;
    helper.m_nSecond = nSecond;

    m_time = helper.toTime();
}

ChaspTime::~ChaspTime()
{
}

////////////////////////////////////////////////////////////////////
// Implementation
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
//! Special operator enabling the ChaspTime object to behave like
//! a \a hasp_time_t type.
////////////////////////////////////////////////////////////////////
ChaspTime::operator hasp_time_t() const
{
    return m_time;
}

////////////////////////////////////////////////////////////////////
//! Returns the day.
////////////////////////////////////////////////////////////////////
unsigned int ChaspTime::day() const
{
    ChaspTimeHelper helper;
    DIAG_VERIFY(helper.toTime(*this));
    
    return helper.m_nDay;
}

////////////////////////////////////////////////////////////////////
//! Returns the hour.
////////////////////////////////////////////////////////////////////
unsigned int ChaspTime::hour() const
{
    ChaspTimeHelper helper;
    DIAG_VERIFY(helper.toTime(*this));
    
    return helper.m_nHour;
}

////////////////////////////////////////////////////////////////////
//! Returns the minute.
////////////////////////////////////////////////////////////////////
unsigned int ChaspTime::minute() const
{
    ChaspTimeHelper helper;
    DIAG_VERIFY(helper.toTime(*this));
    
    return helper.m_nMinute;
}

////////////////////////////////////////////////////////////////////
//! Returns the month.
////////////////////////////////////////////////////////////////////
unsigned int ChaspTime::month() const
{
    ChaspTimeHelper helper;
    DIAG_VERIFY(helper.toTime(*this));
    
    return helper.m_nMonth;
}

////////////////////////////////////////////////////////////////////
//! Returns the second.
////////////////////////////////////////////////////////////////////
unsigned int ChaspTime::second() const
{
    ChaspTimeHelper helper;
    DIAG_VERIFY(helper.toTime(*this));
    
    return helper.m_nSecond;
}

////////////////////////////////////////////////////////////////////
//! Returns the stored time.
////////////////////////////////////////////////////////////////////
hasp_time_t ChaspTime::time() const
{
    return m_time;
}

////////////////////////////////////////////////////////////////////
//! Returns the year.
////////////////////////////////////////////////////////////////////
unsigned int ChaspTime::year() const
{
    ChaspTimeHelper helper;
    DIAG_VERIFY(helper.toTime(*this));
    
    return helper.m_nYear;
}

