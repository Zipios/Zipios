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
 * \brief Implementation of zipios::FileCollection.
 *
 * This file defines all the FileCollection functions that are not
 * pure virtual. The file also includes documentation for all those
 * functions.
 */

#include "zipios++/filecollection.hpp"

#include "zipios++/zipiosexceptions.hpp"

#include <algorithm>


namespace zipios
{



namespace
{

/** \brief A default filename for unnamed collections.
 *
 * This string represents the default m_filename value when a collection
 * is created without a filename.
 */
char const *g_default_filename = "-";


/** \brief Class object used with the std::find_if() function.
 *
 * This function object is used with the STL find_if algorithm to
 * find a FileEntry in a container, which name (as obtained with
 * FileEntry::getName()) is identical to the name specified in the
 * MatchName constructor.
 */
class MatchName
{
public:
    /** \brief Initialize a MatchName object.
     *
     * This function saves the name to search in the FileCollection.
     *
     * This class expect the name to be a full path and file name
     * with extension. The full name has to match.
     *
     * \param[in] name  The name of the file being searched.
     */
    explicit MatchName(std::string const& name)
        : m_name(name)
    {
    }

    /** \brief Compare an entry to this MatchName.
     *
     * This function compares the full name of the entry with the
     * saved full name. If equal, then it returns true. It is used
     * with the std::find_if() function.
     *
     * \todo
     * We could transform that with lambda at some point.
     *
     * \param[in] entry  The entry to compare with the MatchName.
     *
     * \return true if the name of the entry matches the MatchName.
     */
    bool operator() (FileEntry::pointer_t entry) const
    {
        return entry->getName() == m_name;
    }

private:
    std::string const       m_name;
};


/** \brief Class object used with the std::find_if() function.
 *
 * This function object is used with the STL find_if algorithm to
 * find a FileEntry in a container, which name (as obtained with
 * FileEntry::getFileName()) is identical to the name specified in the
 * MatchFileName constructor.
 *
 * \warning
 * The file name cannot include a '/' in this case or the search will
 * always fail.
 */
class MatchFileName
{
public:
    /** \brief Initialize a MatchFileName object.
     *
     * This function saves the base name to search in the
     * FileCollection.
     *
     * This class expect the name to be a base file name, eventually with
     * an extension. If the name includes a slash then the search will
     * always fail.
     *
     * \param[in] name  The name of the file being searched.
     */
    explicit MatchFileName(std::string const& name)
        : m_name(name)
    {
    }

    /** \brief Compare an entry to this MatchFileName.
     *
     * This function compares the base name of the entry with the
     * saved base name. If equal, then it returns true. It is used
     * with the std::find_if() function.
     *
     * \todo
     * We could transform that with lambda at some point.
     *
     * \param[in] entry  The entry to compare with the MatchFileName.
     *
     * \return true if the name of the entry matches the MatchFileName.
     */
    bool operator() (FileEntry::pointer_t entry) const
    {
        return entry->getFileName() == m_name;
    }

private:
    std::string const       m_name;
};


} // no name namespace



/** \class FileCollection
 * \brief Base class for various file collections.
 *
 * FileCollection is an abstract baseclass that represents a
 * collection of files. The specializations of FileCollection
 * represents different origins of file collections, such as
 * directories, simple filename lists and compressed archives.
 */


/** \typedef std::shared_ptr<std::istream> FileCollection::stream_pointer_t; 
 * \brief A shared pointer to an input stream.
 *
 * This type of pointer is used whenever you retrieve an input stream
 * from a file collection such as the ZipFile class. Having shared
 * pointers ensures that the pointers can be shared between various
 * functions and it gets deleted in the end.
 */


/** \fn stream_pointer_t FileCollection::getInputStream(std::string const& entry_name, MatchPath matchpath = MatchPath::MATCH);
 * \brief Retrieve pointer to an istream.
 *
 * This function returns a shared pointer to an istream defined from the
 * named entry which is expected to be available in this collection.
 *
 * The function returns a NULL pointer if there is no entry with the
 * specified name in this FileCollection.
 *
 * Note that the function returns a smart pointer to an istream. In
 * general the FileCollection will not hold that pointer meaning that
 * if you call getInputStream() multiple times with the same
 * \p entry_name parameter, you get distinct istream instances each
 * time.
 *
 * By default the \p entry_name parameter is expected to match the full
 * path and filename (MatchPath::MATCH). If you are looking for a file
 * and want to ignore the directory name, set the matchpath parameter
 * to MatchPath::IGNORE.
 *
 * \warning
 * In version 1.0 there was a version of the function accepting a
 * FileEntry instead of a filename. That function was simply calling
 * this function with file_entry->getName() and MatchPath::MATCH so
 * you can convert the call with:
 *
 * \code
 *      // old code:
 *      ConstEntryPointer ent = zf.getEntry("file2.txt", FileCollection::IGNORE);
 *      if(ent)
 *      {
 *          auto_ptr<istream> is(getInputStream(ent));
 *          if(is)
 *          {
 *              // got access to the file in the archive
 *              ...
 *          }
 *      }
 *
 *      // new code:
 *      zipios::FileEntry::pointer_t ent(zf.getEntry(argv[2], zipios::FileCollection::MatchPath::IGNORE));
 *      if(ent)
 *      {
 *          zipios::ZipFile::stream_pointer_t is(zf.getInputStream(ent->getName()));
 *          if(is)
 *          {
 *              // got access to the file in the archive
 *              ...
 *          }
 *      }
 * \endcode
 *
 * \par
 * There are two reasons for the change: (1) the function really just called
 * the other with getName() and thus there was no reason to have two
 * functions; and (2) the function did NOT test whether the entry was one
 * that this collection owned making it feel like you could call the
 * getInputStream() function of collection A with entry of collection B
 * and still get a valid stream.
 *
 * \param[in] entry_name  The name of the file to search in the collection.
 * \param[in] matchpath  Whether the full path or just the filename is matched.
 *
 * \return A shared pointer to an open istream for the specified entry.
 *
 * \sa CollectionCollection
 * \sa DirectoryCollection
 * \sa ZipFile
 */


/** \fn FileCollection::pointer_t FileCollection::clone() const;
 * \brief Create a clone of this object.
 *
 * This function creates a heap allocated clone of the object this
 * method is called for.
 *
 * \return A shared pointer to a copy of the object this method is called for.
 */


/** \brief Initializes a FileCollection object.
 *
 * This FileCollection constructor initializes the object and
 * mark it as invalid. In most cases an invalid collection cannot
 * be used for anything. You may make it valid by copying a valid
 * collection in it.
 *
 * By default the FileCollection is given the special filename "-".
 *
 * The collection is empty and marked as invalid.
 */
FileCollection::FileCollection(std::string const& filename)
    : m_filename(filename.empty() ? g_default_filename : filename)
    //, m_entries() -- auto-init
    //, m_valid(false) -- auto-init
{
}


/** \brief Copy a FileCollection in a new one.
 *
 * This constructor copies a file collection (\p src) in a new collection.
 *
 * The copy entries that all the entries from the source collection get
 * cloned in the copy. This means entries in the source or new collection
 * can be modified and it has no effect on the entries in the other
 * collection.
 *
 * \param[in] src  The source collection to copy in this collection.
 */
FileCollection::FileCollection(FileCollection const& src)
    : m_filename(src.m_filename)
    //, m_entries() -- see below
    , m_valid(src.m_valid)
{
    m_entries.reserve(src.m_entries.size());
    for(auto it = src.m_entries.begin(); it != src.m_entries.end(); ++it)
    {
        m_entries.push_back((*it)->clone());
    }
}


/** \brief Replace the content of a collection with a copy of another collection.
 *
 * This function copies the \p rhs collection in this collection.
 *
 * Note that the entries in the this collection get released. If you still
 * have a reference to them in a shared pointer, they will not be deleted.
 *
 * The entries in \p rhs get cloned so modifying the entries in the source
 * or the destination has no effect on the entries of the other collection.
 *
 * \param[in] rhs  The source FileCollection to copy.
 *
 * \return A reference to this FileCollection object.
 */
FileCollection& FileCollection::operator = (FileCollection const& rhs)
{
    if(this != &rhs)
    {
        m_filename = rhs.m_filename;

        m_entries.clear();
        m_entries.reserve(rhs.m_entries.size());
        for(auto it(rhs.m_entries.begin()); it != rhs.m_entries.end(); ++it)
        {
            m_entries.push_back((*it)->clone());
        }

        m_valid = rhs.m_valid;
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
 *
 * \note
 * Note that the entries generally get released when this
 * destructor is called. However, since we are using shared
 * pointers, you may still hold valid pointers to the entries
 * even after the FileCollection destructor was called.
 */
FileCollection::~FileCollection()
{
}


/** \brief Close the current FileEntry of this FileCollection.
 *
 * This function closes the current file entry.
 */
void FileCollection::close()
{
    m_entries.clear();
    m_filename = g_default_filename;
    m_valid = false;
}


/** \brief Retrieve the array of entries.
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
 *
 * This function returns a shared pointer to a FileEntry object for
 * the entry with the specified name. To ignore the path part of the
 * filename while searching for a match, specify FileCollection::IGNORE
 * as the second argument.
 *
 * \note
 * The collection must be valid or the function raises an exception.
 *
 * \param[in] name  A string containing the name of the entry to get.
 * \param[in] matchpath  Speficy MatchPath::MATCH, if the path should match
 *                       as well, specify MatchPath::IGNORE, if the path
 *                       should be ignored.
 *
 * \return A shared pointer to the found entry. The returned pointer
 *         is null if no entry is found.
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
        throw InvalidStateException("Attempted to access an invalid FileCollection");
    }
}


/** \brief Write a FileCollection to the output stream.
 *
 * This function writes a simple textual representation of this
 * FileCollection to the output stream.
 *
 * \param[in,out] os  The output stream.
 * \param[in] collection  The collection to print out.
 *
 * \return A reference to the \p os output stream.
 */
std::ostream& operator << (std::ostream& os, FileCollection const& collection)
{
    os << "collection '" << collection.getName() << "' {";
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

// Local Variables:
// mode: cpp
// indent-tabs-mode: nil
// c-basic-offset: 4
// tab-width: 4
// End:
