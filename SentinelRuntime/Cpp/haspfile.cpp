////////////////////////////////////////////////////////////////////
// Copyright (C) 2011, SafeNet, Inc. All rights reserved.
//
// HASP(R) is a registered trademark of SafeNet, Inc.  
//
//
// $Id: haspfile.cpp,v 1.34 2011-05-02 03:12:07 nsingh1 Exp $
////////////////////////////////////////////////////////////////////
#include "hasp_api_cpp_.h"


////////////////////////////////////////////////////////////////////
//! \class ChaspFile hasp_api_cpp.h
//! \brief ChaspFile enables file operations on a HASP protection key.
//!
/////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
// Construction/Destruction
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
//! Initializes the object.
////////////////////////////////////////////////////////////////////
ChaspFile::ChaspFile()
{
}

////////////////////////////////////////////////////////////////////
//! Initializes the object to use the file \a fileId of the HASP
//! protection key specified by \a other.
////////////////////////////////////////////////////////////////////
ChaspFile::ChaspFile(hasp_fileid_t fileId, const Chasp& other)
    : ChaspBase(other)
{
    init(fileId);
}

////////////////////////////////////////////////////////////////////
//! Copy constructor.
////////////////////////////////////////////////////////////////////
ChaspFile::ChaspFile(const ChaspFile& other)
    : ChaspBase(other)
{
    init(other.getFileId());
}

ChaspFile::~ChaspFile()
{
    DIAG_VERIFY(release());
}

////////////////////////////////////////////////////////////////////
// Implementation
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
//! Assignment operator.
////////////////////////////////////////////////////////////////////
ChaspFile& ChaspFile::operator=(const ChaspFile& other)
{
    if (this != &other)
    {
        dynamic_cast<ChaspBase&>(*this) =
            dynamic_cast<const ChaspBase&>(other);
    
        init(other.getFileId());
    }

    return *this;
}

////////////////////////////////////////////////////////////////////
//! Determines whether the string \a szString can be written
////////////////////////////////////////////////////////////////////
bool ChaspFile::canWriteString(const char* szString)
{
    return (NULL == szString) ? true : 
                                canWriteString(std::string(szString));
}

////////////////////////////////////////////////////////////////////
//! Determines whether the string \a string can be written
////////////////////////////////////////////////////////////////////
bool ChaspFile::canWriteString(const std::string& string)
{
    return maxStringLength() >= string.length();
}

////////////////////////////////////////////////////////////////////
//! Returns the file identifier used for read/write operations on
//! the HASP protection key.
////////////////////////////////////////////////////////////////////
hasp_fileid_t ChaspFile::getFileId() const
{
    DIAG_ASSERT(isValid());
    return m_fileId;
}

////////////////////////////////////////////////////////////////////
//! Returns the current file position.
//!
//! \sa                         setFilePos
////////////////////////////////////////////////////////////////////
hasp_size_t ChaspFile::getFilePos() const
{
    DIAG_ASSERT(isValid());
    return m_ulFilePos;
}

////////////////////////////////////////////////////////////////////
//! Returns the file position for the passed string.
//!
//! \sa                         setFilePos
////////////////////////////////////////////////////////////////////
hasp_size_t ChaspFile::getFilePosFromString(const char* szString)
{
    return getFilePosFromString(std::string((NULL == szString) ? 
                                                "" : szString));
}

////////////////////////////////////////////////////////////////////
//! Returns the file position for the passed string.
//!
//! \sa                         setFilePos
////////////////////////////////////////////////////////////////////
hasp_size_t ChaspFile::getFilePosFromString(const std::string& string)
{
    return canWriteString(string) ? 
                static_cast<hasp_size_t>(string.length() + sizeof(unsigned char)) : 
                0;
}

////////////////////////////////////////////////////////////////////
//! Returns the size of the file.
//!
//! \param ulSize               Reference receiving the file size.
//!
//! \return                     A \a haspStatus status code.
////////////////////////////////////////////////////////////////////
haspStatus ChaspFile::getFileSize(hasp_size_t& ulSize) const
{
    HASP_PROLOGUE(m_handle);
    return HASP_KEYPTR->getFileSize(m_fileId, ulSize);
}

////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////
hasp_u32_t ChaspFile::hashCode() const
{
    hasp_u32_t code = ChaspBase::hashCode();
    if (static_cast<hasp_u32_t>(-1) != code)
    {
        code &= 0xFFFF;
        code |= m_fileId << 0x10;
    }

    return code;
}

////////////////////////////////////////////////////////////////////
//! Initializes the object.
////////////////////////////////////////////////////////////////////
void ChaspFile::init(hasp_fileid_t fileId /* = fileReadWrite */)
{
    m_fileId = fileId;
    m_ulFilePos = 0;
}

////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////
unsigned char ChaspFile::maxStringLength()
{
    return 0xFF;
}

////////////////////////////////////////////////////////////////////
//! Reads data from the current position into a buffer.
//!
//! \param pData                Pointer to a buffer data shall be
//!                             written to.
//! \param ulSize               The size of the buffer in bytes.
//!
//! \return                     A \a haspStatus status code.
//!
//! \sa                         ChaspImpl::readFile
//!                             getFilePos
//!                             setFilePos
////////////////////////////////////////////////////////////////////
haspStatus ChaspFile::read(unsigned char* pData, 
                           hasp_size_t ulSize) const
{
    HASP_PROLOGUE(m_handle);
    return HASP_KEYPTR->readFile(m_fileId,
                                 m_ulFilePos,
                                 pData,
                                 ulSize);
}

////////////////////////////////////////////////////////////////////
//! Reads a std::string from the HASP protection key memory
////////////////////////////////////////////////////////////////////
haspStatus ChaspFile::read(std::string& string) const
{
    string.resize(0);

    unsigned char length = 0;
    haspStatus status = read(&length, sizeof(unsigned char));

    if (!HASP_SUCCEEDED(status))
        return status;

    if (0 < length)
    {
    
        std::vector<unsigned char> vector(length+1);
    
        status = read(&vector[0],
                      static_cast<hasp_size_t>(vector.size() * sizeof(unsigned char)));
        if (!HASP_SUCCEEDED(status))
            return status;

        std::vector<unsigned char>::iterator it = vector.begin();
        
	// Note that we increment the iterator before reading the first byte,
	// therefore the length byte is skipped and will not show up 
	// in the final string
        for (++it; vector.end() != it; it++)
            string += *it;
    }

    return HASP_STATUS_OK;
}

////////////////////////////////////////////////////////////////////
//! Releases the private key this instance points to and 
//! invalidates the object.
//!
//! \return                     \a true on success, \a false
//!                             otherwhise.
//!
//! \sa                         ChaspBase::release
////////////////////////////////////////////////////////////////////
bool ChaspFile::release()
{
    if (!ChaspBase::release())
        return false;

    m_fileId = 0;
    m_ulFilePos = 0;

    return true;
}

////////////////////////////////////////////////////////////////////
//! Sets the file position for the next read/write operation.
//!
//! \param ulPos                The new position within the file.
//!
//! \return                     \a true on success, \a false
//!                             otherwhise.
//!
//! \sa                         getFilePos
//!                             getFileSize
////////////////////////////////////////////////////////////////////
bool ChaspFile::setFilePos(hasp_size_t ulPos)
{
    hasp_size_t ulSize = 0;
    if (!HASP_SUCCEEDED(getFileSize(ulSize)) || (ulSize <= ulPos))
        return false;

    m_ulFilePos = ulPos;
    return true;
}

////////////////////////////////////////////////////////////////////
//! Returns the file identifier as a human readable string.
////////////////////////////////////////////////////////////////////
std::string ChaspFile::toString() const
{
    ChaspMap map;

    DIAG_ASSERT(isValid());
    if (!isValid())
        return std::string();

    std::ostringstream stream;
    stream << m_fileId;

    return std::string(stream.str());
}

////////////////////////////////////////////////////////////////////
//! Writes data from a buffer to the key.
//!
//! \param pData                Pointer to a buffer holding the
//!                             data.
//! \param ulCount              The number of elements to be written.
//!
//! \return                     A \a haspStatus status code.
//!
//! \sa                         ChaspImpl::writeFile
//!                             getFilePos
//!                             setFilePos
////////////////////////////////////////////////////////////////////
haspStatus ChaspFile::write(const unsigned char* pData, 
                            hasp_size_t ulCount) const
{
    HASP_PROLOGUE(m_handle);
    return HASP_KEYPTR->writeFile(m_fileId,
                                  m_ulFilePos,
                                  pData, 
                                  ulCount);
}

////////////////////////////////////////////////////////////////////
//! Writes a string into the HASP protection key memory
////////////////////////////////////////////////////////////////////
haspStatus ChaspFile::write(const char* szData) const
{
    return write(std::string((NULL == szData) ? "" : szData));
}

////////////////////////////////////////////////////////////////////
//! Writes a std::string into the HASP protection key memory
////////////////////////////////////////////////////////////////////
haspStatus ChaspFile::write(const std::string& string) const
{
    if (maxStringLength() < string.length())
        return HASP_INVALID_PARAMETER;

    std::vector<unsigned char> vector;
    vector.push_back(static_cast<unsigned char>(string.length()));
    
    std::copy(string.begin(), 
              string.end(), 
              std::back_inserter(vector));

    return write(&vector[0], 
                 static_cast<hasp_size_t>(vector.size()));
}
