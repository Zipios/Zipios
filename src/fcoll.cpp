/*
  Zipios++ - a small C++ library that provides easy access to .zip files.
  Copyright (C) 2000-2015  Thomas Sondergaard

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
    Implementation of FileCollection.
*/

#include "zipios++/fcoll.h"

#include "zipios++/zipiosexceptions.h"

#include <algorithm>


namespace zipios
{



namespace
{


/** Function object to be used with the STL find_if algorithm to
 * find a FileEntry in a container, which name (as obtained with
 * FileEntry::getName()) is identical to the name specified in the
 * MatchName constructor.
 */
class MatchName
{
public:
    explicit MatchName(std::string const& name)
        : m_name(name)
    {
    }

    bool operator() (FileEntry::pointer_t entry) const
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
class MatchFileName
{
public:
    explicit MatchFileName(std::string const& name)
        : m_name(name)
    {
    }

    bool operator() (FileEntry::pointer_t entry) const
    {
        return entry->getFileName() == m_name;
    }

private:
    std::string const       m_name;
};


} // no name namespace



/** \class FileCollection
 * \anchor fcoll_anchor
 *
 * \brief Base class for various file collections.
 *
 * FileCollection is an abstract baseclass that represents a
 * collection of files. The specializations of FileCollection
 * represents different origins of file collections, such as
 * directories, simple filename lists and compressed archives.
 */


/** \typedef std::shared_ptr<std::istream> stream_pointer_t; 
 * \brief A shared pointer to an input stream.
 *
 * This type of pointer is used whenever you retrieve an input stream
 * from a file collection such as the ZipFile class. Having shared
 * pointers ensures that the pointers can be shared between various
 * functions and it gets deleted in the end.
 */


/** \func void close();
 * \brief Closes the FileCollection.
 *
 * Close the current file entry.
 */


/** \func stream_pointer_t getInputStream(ConstEntryPointer const& entry);
 * \brief Retrieve an input stream pointer.
 * \anchor fcoll_getinputstream
 *
 * The function returns a pointer to an opened istream for the specified
 * FileEntry. It is the caller's responsibility to delete the stream
 * when he is done with it. Returns 0, if there is no such
 * FileEntry in the FileCollection.
 *
 * \param entry  A ConstEntryPointer to the FileEntry to get an istream to.
 *
 * \return An open istream for the specified entry. The istream is
 *         allocated on heap and it is the users responsibility to
 *         delete it when he is done with it.
 */



/** \func stream_pointer_t getInputStream(std::string const& entry_name, MatchPath matchpath = MatchPath::MATCH);
 * \brief Retrieve pointer to istream.
 *
 * This function returns the pointer to the named istream defined in
 * by this collection.
 *
 * It is the caller's responsibility to delete the stream when he is
 * done with it. Returns nullptr if there is no entry with the specified
 * name in the FileCollection.
 *
 * \param[in] entry_name  The name of the file to search in the collection.
 * \param[in] matchpath  Speficy MatchPath::MATCH, if the path should
 *                       match as well, specify MatchPath::IGNORE, if
 *                       the path should be ignored.
 *
 * \return An open istream for the specified entry. The istream is
 *         allocated on heap and it is the caller's responsibility to
 *         delete it when he is done with it.
 */



/** \func FileCollection *clone() const;
 * \brief Create a clone of this object.
 *
 * This function creates a heap allocated clone of the object this method is called for. The
 * caller is responsible for deallocating the clone when he is done with it.
 *
 * \return A heap allocated copy of the object this method is called for.
 */


/** FileCollection constructor.
 *
 */
FileCollection::FileCollection()
    : m_filename("-")
    , m_entries(0)
    , m_valid(false)
{
}

/**
 *
 * Copy constructor.
 */
FileCollection::FileCollection(FileCollection const& src)
    : m_filename(src.m_filename)
    , m_valid(src.m_valid)
{
    m_entries.reserve(src.m_entries.size());
    for(auto it = src.m_entries.begin(); it != src.m_entries.end(); ++it)
    {
        m_entries.push_back((*it)->clone());
    }
}


/** Copy assignment operator. */
FileCollection const& FileCollection::operator = (FileCollection const& src)
{
    if(this != &src)
    {
        m_filename = src.m_filename;
        m_valid = src.m_valid;

        m_entries.clear() ;
        m_entries.reserve( src.m_entries.size() ) ;

        for(auto it(src.m_entries.begin()); it != src.m_entries.end(); ++it)
        {
            m_entries.push_back((*it)->clone());
        }
    }

    return *this;
}


/** \brief Make sure the resources are released.
 *
 * The FileCollection destructor makes sure that any resources
 * still allocated get released.
 *
 * For example, the ZipFile implementation calls the close()
 * function.
 */
FileCollection::~FileCollection()
{
}



/** \brief Retrieve the array of entries.
 * \anchor fcoll_entries_anchor
 *
 * This function returns a copy of the file collection vector of entries.
 * Note that the vector is copied but not the entries, so modifications
 * to the entries will be reflected in this FileCollection entries.
 * However, adding and removing entries to the collection is not
 * reflected in the copy.
 *
 * \return A vector containing the entries of this FileCollection.
 */
FileEntry::vector_t FileCollection::entries() const
{
    mustBeValid();

    return m_entries;
}


/** \brief Get an entry from this collection.
 * \anchor fcoll_getentry_anchor
 *
 * This function returns a ConstEntryPointer to a FileEntry object for
 * the entry with the specified name. To ignore the path part of the
 * filename in search of a match, specify FileCollection::IGNORE as
 * the second argument.
 *
 * \note
 * The collection my be valid.
 *
 * \param name  A string containing the name of the entry to get.
 * \param matchpath  Speficy MatchPath::MATCH, if the path should match
 *                   as well, specify MatchPath::IGNORE, if the path
 *                   should be ignored.
 *
 * \return A ConstEntryPointer to the found entry. The returned pointer
 *         equals zero if no entry is found.
 *
 * \sa mustBeValid()
 */
FileEntry::pointer_t FileCollection::getEntry(std::string const& name, MatchPath matchpath) const
{
    mustBeValid();

    FileEntry::vector_t::const_iterator iter;
    if(matchpath == MatchPath::MATCH)
    {
        iter = std::find_if(m_entries.begin(), m_entries.end(), MatchName(name));
    }
    else
    {
        iter = std::find_if(m_entries.begin(), m_entries.end(), MatchFileName(name));
    }

    return iter == m_entries.end() ? FileEntry::pointer_t() : *iter;
}


/** \brief Returns the name of the FileCollection.
 *
 * This function returns the filename of the collection as a whole.
 *
 * \note
 * The collection my be valid.
 *
 * \return The name of the FileCollection.
 *
 * \sa mustBeValid()
 */
std::string FileCollection::getName() const
{
    mustBeValid();
    return m_filename;
}


/** \brief Returns the number of entries in the FileCollection.
 *
 * This function returns the number of entries in the collection.
 *
 * \note
 * The collection my be valid.
 *
 * \return The number of entries in the FileCollection.
 *
 * \sa mustBeValid()
 */
size_t FileCollection::size() const
{
    mustBeValid();
    return m_entries.size();
}


/** \brief Check whether the current collection is valid.
 *
 * This function returns true if the collection is valid.
 *
 * Note that by default (just after a new) a collection is
 * not considered valid.
 *
 * \return true if the collection is valid.
 */
bool FileCollection::isValid() const
{
    return m_valid;
}


/** \brief Check whether the collection is valid.
 *
 * This function verifies that the collection is valid. If not, an
 * exception is raised. Many other functions from the various collection
 * functions are calling this function before accessing data.
 *
 * \exception InvalidStateException
 * This exception is raised if the m_valid field is currently false and
 * thus most of the collection data is considered invalid.
 */
void FileCollection::mustBeValid() const
{
    if(!m_valid)
    {
        throw InvalidStateException("Attempted to access an invalid CollectionCollection");
    }
}


std::ostream& operator << (std::ostream& os, FileCollection const& collection)
{
    os << "collection '" << collection.getName() << "' {" ;
    FileEntry::vector_t entries(collection.entries());
    char const *sep("");
    for(auto it = entries.begin(); it != entries.end(); ++it)
    {
        os << sep;
        sep = ", ";
        os << (*it)->getName();
    }
    os << "}";
    return os;
}



} // zipios namespace
// vim: ts=4 sw=4 et
