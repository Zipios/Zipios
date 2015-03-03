#pragma once
/*
  Zipios++ - a small C++ library that provides easy access to .zip files.
  Copyright (C) 20000-2015  Thomas Sondergaard

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
*/

/** \file
    Header file that defines FileEntry.
*/

#include "zipios++/meta-iostreams.h"

#include "zipios++/simplesmartptr.h"

#include <string>
#include <vector>
#include <ctime>


namespace zipios
{


/** \brief The types used with FileEntry::setMethod and FileEntry::getMethod.
 *
 * The current entries are the types supported by the zip format. The
 * numbering matches one to one the numbering used in the zip file format,
 * i.e. STORED is indicated by a 0 in the method field in a zip file and
 * so on.
 */
enum class StorageMethod : uint8_t
{
  STORED = 0,
  SHRUNK,
  REDUCED1,
  REDUCED2,
  REDUCED3,
  REDUCED4,
  IMPLODED,
  RESERVED,
  DEFLATED
};



/** \brief A FileEntry represents an entry in a FileCollection.
 *
 * The interface is a copy of the ZipEntry interface from the java.util.zip
 * package. The name has been changed to FileEntry, as FileCollection
 * is a more general abstraction, that covers other types of file
 * collections than just zip files.
 */
class FileEntry
{
public:

    /* Default constructor, copy constructor, and copy assignment
       operator are sufficient. */

    /** FileEntry destructor. */
    virtual ~FileEntry() {}

    /** Returns the comment of the entry, if it has one. Otherwise it
      returns an empty string.
      @return the comment associated with the entry, if there is one.
     */
    virtual std::string getComment() const = 0;

    /** Returns the compressed size of the entry. If the entry is not
      stored in a compressed format, the uncompressed size is
      returned.
      @return the compressed size of the entry. If the entry is stored without
      compression the uncompressed size is returned.
     */
    virtual uint32_t getCompressedSize() const = 0;

    /** Returns the Crc for the entry, if it has one. FIXME: what is
      returned if it doesn't have one?
      @return the Crc for the entry, if it has one.
     */
    virtual uint32_t getCrc() const = 0;

    /** Returns a vector of bytes of extra data that may be stored with
      the entry.
      @return A vector< unsigned char > of extra bytes that may potentially
      be associated with an entry.
     */
    virtual std::vector< unsigned char > getExtra() const = 0;

    /** Returns the method used to store the entry in the FileCollection.
      @return the storage method used to store the entry in the collection.
      @see StorageMethod.
     */
    virtual StorageMethod getMethod() const = 0;

    /** Returns the full filename of the entry, including a path if the
      entry is stored in a subfolder.
      @return the filename of the entry, including path if the entry is stored
      in a sub-folder.
     */
    virtual std::string getName() const = 0;

    /** Returns the filename of the entry.
      @return Returns the filename of the entry.
     */
    virtual std::string getFileName() const = 0;

    /** Returns the (uncompressed) size of the entry data.
      @return Returns the (uncompressed) size of the entry.
     */
    virtual uint32_t getSize() const = 0;

    /** Returns the date and time of the entry in MSDOS date/time format.
      @return the date and time of the entry.
     */
    virtual int getTime() const = 0;

    /** Returns the date and time of the entry in Unix date/time format (see time()).
      @return the date and time of the entry.
     */
    virtual std::time_t getUnixTime() const = 0;

    /** Any method or operator that initializes a FileEntry may set a
      flag, that specifies whether the read entry is valid or not. If
      it isn't this method returns false.
      @return true if the FileEntry has been parsed succesfully.
     */
    virtual bool isValid() const = 0;

    //     virtual int hashCode() const = 0;

    /** Returns true if the entry is a directory. A directory entry is
      an entry which name ends with a separator ('/' for Unix systems,
      '\' for Windows and DOS systems.
      @return true if the entry is a directory.
     */
    virtual bool isDirectory() const = 0;

    /** Sets the comment field for the FileEntry.
      @param comment string with the new comment.
     */
    virtual void setComment(std::string const& comment) = 0;

    /** Set the compressed size field of the entry.
      @param size value to set the compressed size field of the entry to.
     */
    virtual void setCompressedSize(uint32_t size) = 0;

    /** Sets the crc field.
      @param crc value to set the crc field to.
     */
    virtual void setCrc(uint32_t crc) = 0;

    /** Sets the extra field.
      @param extra the extra field is set to this value.
     */
    virtual void setExtra(std::vector<unsigned char> const& extra) = 0;

    /** Sets the storage method field for the entry.
      @param method the method field is set to the specified value.
     */
    virtual void setMethod(StorageMethod method) = 0;

    /** Sets the name field for the entry.
      @param name the name field is set to the specified value.
     */
    virtual void setName(std::string const& name) = 0;

    /**   Sets the size field for the entry.
      @param size the size field is set to this value.
     */
    virtual void setSize(uint32_t size) = 0;

    /** Sets the time field for the entry.
      @param time set time field as is using this MSDOS date/time formatted value.
     */
    virtual void setTime(int time) = 0;

    /** Sets the time field in Unix time format for the entry.
      @param time the time field is set to the specified value.
     */
    virtual void setUnixTime(std::time_t time) = 0;

    /** Returns a human-readable string representation of the entry.
      @return a human-readable string representation of the entry.
     */
    virtual std::string toString() const = 0;

    /** Create a heap allocated clone of the object this method is called for. The
      caller is responsible for deallocating the clone when he is done with it.
      @return A heap allocated copy of the object this method is called for.
     */
    virtual FileEntry *clone() const = 0;

    // TBD: could these be protected?
    class MatchName;
    class MatchFileName;

protected:
    friend class SimpleSmartPointer< FileEntry > ;
    friend class SimpleSmartPointer< const FileEntry > ;

    void ref() const
    {
        _refcount.ref() ;
    }

    unsigned int unref() const
    {
        return _refcount.unref() ;
    }

    ReferenceCount< FileEntry > _refcount;
};


/** \brief EntryPointer is a SimpleSmartPointer for a FileEntry.
 */
typedef SimpleSmartPointer<FileEntry> EntryPointer;


/** \brief ConstEntryPointer is a SimpleSmartPointer for a constant FileEntry.
 */
typedef SimpleSmartPointer<const FileEntry> ConstEntryPointer;


/** \brief Entries is a vector of EntryPointer pointers.
 */
typedef std::vector<EntryPointer> Entries;


/** ConstEntries is a vector of ConstEntryPointer's
 */
typedef std::vector<EntryPointer> ConstEntries;


/** Function object to be used with the STL find_if algorithm to
    find a FileEntry in a container, which name (as obtained with
    FileEntry::getName()) is identical to the name specified in the
    MatchName constructor. */
class FileEntry::MatchName
{
public:
    explicit MatchName(std::string const& name)
        : m_name(name)
    {
    }

    bool operator() (ConstEntryPointer const& entry) const
    {
        return entry->getName() == m_name;
    }

private:
    std::string const       m_name;
};


/** Function object to be used with the STL find_if algorithm to
 * find a FileEntry in a container, which name (as obtained with
 * FileEntry::getFileName()) is identical to the name specified in the
 * MatchName constructor.
 */
class FileEntry::MatchFileName
{
public:
    explicit MatchFileName(std::string const& name)
        : m_name(name)
    {
    }

    bool operator() (ConstEntryPointer const& entry) const
    {
        return entry->getFileName() == m_name;
    }

private:
    std::string const       m_name;
};


std::ostream& operator << (std::ostream &os, FileEntry const& entry);


inline std::ostream& operator << (std::ostream &os, ConstEntryPointer const& entry)
{
    os << *entry;
    return os;
}



} // zipios namespace

// vim: ts=4 sw=4 et
