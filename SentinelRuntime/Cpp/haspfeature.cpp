////////////////////////////////////////////////////////////////////
// Copyright (C) 2011, SafeNet, Inc. All rights reserved.
//
// HASP(R) is a registered trademark of SafeNet, Inc. 
//
//
// $Id: haspfeature.cpp,v 1.19 2011-05-02 03:12:07 nsingh1 Exp $
////////////////////////////////////////////////////////////////////
#include "hasp_api_cpp_.h"


////////////////////////////////////////////////////////////////////
//! \mainpage Overview
//!
//! This document describes Sentinel HASP high layer API.
//!
//! \section __helpers__ Helper Classes
//!
//! For the sake of convenience several helper classes are 
//! implemented. These allow a more flexible access to certain
//! parameters required by the low level API.
//!
//! \subsection __ChaspFeature__ The ChaspFeature Class
//!
//! The ChaspFeature class encapsulates a feature identifier of a 
//! HASP protection key feature.
//!
//! \subsection __ChaspTime__ The ChaspTime Class
//!
//! This class encapsulates the date/time functionality provided 
//! by a HASP protection key.
//!
//! \subsection __ChaspHandle__ The ChaspHandle Class
//!
//! This class is used internally by the ChaspBase class. ChaspHandle 
//! holds the the index into the Chasp - ChaspImpl map and some
//! state informations in addition. A unique usage counter of the
//! the index in the map and a logout counter is used to check if a 
//! logout occured.
//!
//! \section __key__ The Key Classes
//!
//! This section describes the classes for accessing a Hasp protection
//! key. 
//! The basic concept of the implementation is to provide a private
//! class implementing all methods for accessing a Hasp protection key 
//! via the low level API. The access to this class is controlled via
//! a helper mapping class maintaining a lock mechanism, which 
//! permits the access to the private class from different threads.
//! The design of the public interface is straight foreward.
//! Main concept is to provide classes, which just act as wrappers.
//! The advantage of this design is, that these classes can be
//! easily replaced (i.e. for implementing a .net assembly or a 
//! COM interface).
//! The Base of these classes is the ChaspBase class.
//! The implementation of this class is pure virtual. Main purpose 
//! of it is to maintain the mapping to the private key class.
//!
//! \subsection __ChaspBase__ The ChaspBase Class
//!
//! This is a pure virtual class. No instance shall ever directly
//! be created from this class. Purpose of this class is to provide
//! methods for creating a, attaching to and detaching from a 
//! private key object via the map object.
//!
//! \subsection __Chasp__ The Chasp class
//! 
//! The Chasp class represents the main public class. Through
//! this class a Hasp protection key can be accessed. There
//! can be any number of instances referencing the same 
//! Hasp protection key.
//!
//! In addition this class provides the methods for gaining access
//! to the Hasp key's file operation class \ref ChaspFile and the 
//! legacy class \ref ChaspLegacy 
//! (for old Hasp4 compatible actions).
//!
//! \subsection __ChaspFile__ The ChaspFile class
//!
//! The ChaspFile class is used for file i/o operations on a 
//! Hasp protection key. Objects of this class are instantiated via
//! the Chasp class.
//!
//!
//! \subsection __ChaspLegacy__ The ChaspLegacy class
//!
//! The ChaspLegacy class is used to get access to the legacy
//! functions of a Hasp4. Objects of this class are instatiated
//! via the Chasp class.
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
//! \struct ChaspFeature hasp_api_cpp.h
//! \brief Class holding a Hasp key feature id.
//!
//! The ChaspFeature class holds the feature identifier of a Hasp 
//! key. Furthermore methods are implemented allowing the feature
//! to be manipulated. Through the static method 
//! \ref fromProgramNumber an old Hasp4 program number (FAS) can be 
//! converted into a Hasp key feature.
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
// Construction/Destruction
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
//! Constructs a ChaspFeature object with the feature identifier
//! \a ulFeature.
////////////////////////////////////////////////////////////////////
ChaspFeature::ChaspFeature(hasp_u32_t ulFeature)
    : m_ulFeature(ulFeature)
{
}

////////////////////////////////////////////////////////////////////
//! Destroys the object.
////////////////////////////////////////////////////////////////////
ChaspFeature::~ChaspFeature()
{
}

////////////////////////////////////////////////////////////////////
// Implementation
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
//!
////////////////////////////////////////////////////////////////////
ChaspFeature ChaspFeature::defaultFeature()
{
    return ChaspFeature(HASP_DEFAULT_FID);
}

////////////////////////////////////////////////////////////////////
//! Returns the feature identifier.
////////////////////////////////////////////////////////////////////
hasp_u32_t ChaspFeature::feature() const
{
    return m_ulFeature;
}

////////////////////////////////////////////////////////////////////
//! Special operator enabling the ChaspFeature to behave like a
//! \a hasp_u32_t primitive type.
////////////////////////////////////////////////////////////////////
ChaspFeature::operator hasp_feature_t() const
{
    return m_ulFeature;
}

////////////////////////////////////////////////////////////////////
//! Strips the options from a classic Hasp protection key feature, 
//! else does nothing.
////////////////////////////////////////////////////////////////////
hasp_u32_t ChaspFeature::featureId() const
{
    return isProgNum() ? m_ulFeature & ~HASP_PROGNUM_OPT_MASK :
                         m_ulFeature;
}

////////////////////////////////////////////////////////////////////
//! Creates and returns a ChaspFeature instance based on a 
//! feature id
//!
//! \param ulFeature            The feature id to be used
//! \return                     A ChaspFeature class 
//!                             If the feature is a program number
//!                             the default feature will be returned
////////////////////////////////////////////////////////////////////
ChaspFeature ChaspFeature::fromFeature(hasp_u32_t ulFeature)
{
    if (HASP_PROGNUM_FEATURETYPE == (ulFeature & HASP_FEATURETYPE_MASK))
        return ChaspFeature::defaultFeature();
    
    return ChaspFeature(ulFeature);
}

////////////////////////////////////////////////////////////////////
//! Converts an old Hasp4 program number (FAS) into a Hasp protection
//! key compliant feature identifier.
//!
//! \param ulProgNumber         The Hasp4 program number to
//!                             be converted.
//! \return                     A feature class for a Hasp4 program 
//!                             number.
////////////////////////////////////////////////////////////////////
ChaspFeature ChaspFeature::fromProgNum(hasp_u32_t ulProgNum)
{
    ulProgNum &= HASP_PROGNUM_MASK;
    return ChaspFeature(HASP_PROGNUM_FEATURETYPE|ulProgNum);
}

////////////////////////////////////////////////////////////////////
//! Returns the set options of a legacy Hasp4 feature.
////////////////////////////////////////////////////////////////////
hasp_u32_t ChaspFeature::getOptions() const
{
    return isProgNum() ? m_ulFeature & HASP_PROGNUM_OPT_MASK :
                         optDefault;
}

////////////////////////////////////////////////////////////////////
//! Returs whether the option \a ulOption is present in the feature.
//!
//! \note                       This method is only applicable for 
//!                             Hasp4 features. For Hasp protection 
//!                             key features the method always returns
//!                             \a false.
////////////////////////////////////////////////////////////////////
bool ChaspFeature::hasOption(hasp_u32_t ulOption) const
{
    if (!isProgNum())
        return optDefault == ulOption;

    hasp_u32_t ulOptions = m_ulFeature & HASP_PROGNUM_OPT_MASK;
    return (optDefault == ulOption) ? 
                (optDefault == ulOptions) :
                (ulOption == (ulOptions & ulOption));
}

////////////////////////////////////////////////////////////////////
//! Returns \a true if the feature is the default HASP key or the
//! default classic feature.
////////////////////////////////////////////////////////////////////
bool ChaspFeature::isDefault() const
{
    return featureId() == static_cast<hasp_u32_t>(isProgNum() ? 
                                                    HASP_PROGNUM_DEFAULT_FID :
                                                    HASP_DEFAULT_FID);
}

////////////////////////////////////////////////////////////////////
//! Returns \a true if the feature is a classic Hasp4 feature.
////////////////////////////////////////////////////////////////////
bool ChaspFeature::isProgNum() const
{
    return HASP_PROGNUM_FEATURETYPE == (m_ulFeature & HASP_FEATURETYPE_MASK);
}

////////////////////////////////////////////////////////////////////
//!
////////////////////////////////////////////////////////////////////
ChaspFeature ChaspFeature::progNumDefault()
{
    return ChaspFeature(HASP_PROGNUM_DEFAULT_FID);
}

////////////////////////////////////////////////////////////////////
//! Sets or removes options for a classic Hasp4 feature.
//!
//! \param ulAdd                The options to be added.
//! \param ulRemove             The options to be removed.
//!
//! \return                     \a true on success, \a false 
//!                             otherwise.
//!
//! \note                       The method always returns \a false 
//!                             if the feature is HASP protection key feature.
////////////////////////////////////////////////////////////////////
bool ChaspFeature::setOptions(hasp_u32_t ulAdd, hasp_u32_t ulRemove)
{
    if (!isProgNum())
        return false;

    if (optNotRemote == (ulAdd & optNotRemote))
    {
        ulAdd &= ~optNotLocal;
        ulRemove |= optNotLocal;
    }

    if (optNotLocal == (ulAdd & optNotLocal))
    {
        ulAdd &= ~optNotRemote;
        ulRemove |= optNotRemote;
    }

    m_ulFeature |= ulAdd & HASP_PROGNUM_OPT_MASK;
    m_ulFeature &= ~(ulRemove & HASP_PROGNUM_OPT_MASK);
    
    return true;
}

////////////////////////////////////////////////////////////////////
//! Returns the feature identifier as a human readable string.
////////////////////////////////////////////////////////////////////
std::string ChaspFeature::toString() const
{
    std::ostringstream stream;
    stream << m_ulFeature;

    return std::string(stream.str());
}
