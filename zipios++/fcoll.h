#pragma once
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
    Header file that defines FileCollection.
*/

#include "zipios++/fileentry.h"


namespace zipios
{


/** \anchor fcoll_anchor
 * \brief Base class for various file collections.
 *
 * FileCollection is an abstract baseclass that represents a
 * collection of files. The specializations of FileCollection
 * represents different origins of file collections, such as
 * directories, simple filename lists and compressed archives.
 */
class FileCollection
{
public:
    /** FileCollection constructor. */
    explicit FileCollection()
        : m_filename("-")
        , m_entries(0)
        , m_valid(false)
    {
    }

    /** Copy constructor. */
    FileCollection(FileCollection const& src);

    /** Copy assignment operator. */
    const FileCollection& operator = (FileCollection const& src);

    virtual ~FileCollection();

    /** Closes the FileCollection. */
    virtual void close() = 0;

    /** \anchor fcoll_entries_anchor
      Returns a vector of const pointers to the entries in the
      FileCollection.  
      @return a ConstEntries
      containing the entries of the FileCollection. 
      @throw InvalidStateException Thrown if the collection is invalid. */
    virtual ConstEntries entries() const;

    enum class MatchPath : uint32_t
    {
        IGNORE,
        MATCH
    };

    /** \anchor fcoll_getentry_anchor
     * Returns a ConstEntryPointer to a FileEntry object for the entry 
     * with the specified name. To ignore the path part of the filename in search of a
     * match, specify FileCollection::IGNORE as the second argument.
     * @param name A string containing the name of the entry to get.
     * @param matchpath Speficy MATCH, if the path should match as well, 
     * specify IGNORE, if the path should be ignored.
     * @return A ConstEntryPointer to the found entry. The returned pointer
     * equals zero if no entry is found.
     * @throw InvalidStateException Thrown if the collection is invalid.
     */
    virtual ConstEntryPointer getEntry(std::string const& name, 
                                       MatchPath matchpath = MatchPath::MATCH) const;

    /** \anchor fcoll_getinputstream
      Returns a pointer to an opened istream for the specified
      FileEntry. It is the callers responsibility to delete the stream
      when he is done with it. Returns 0, if there is no such
      FileEntry in the FileCollection.
      @param entry A ConstEntryPointer to the FileEntry to get an istream to.
      @return an open istream for the specified entry. The istream is allocated on
      heap and it is the users responsibility to delete it when he is done with it.
      @throw InvalidStateException Thrown if the collection is invalid. */
    virtual std::istream *getInputStream(ConstEntryPointer const& entry) = 0;

    /** \brief Retrieve pointer to istream.
     *
     * This function returns the pointer to the named istream defined in
     * by this collection.
     *
     * It is the callers responsibility to delete the stream when he is
     * done with it. Returns nullptr if there is no entry with the specified
     * name in the FileCollection.
     *
     * \exception InvalidStateException
     * Raised if the collection is invalid.
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
    virtual std::istream *getInputStream(std::string const& entry_name, 
                                         MatchPath matchpath = MatchPath::MATCH ) = 0;

    /** Returns the name of the FileCollection.
      @return the name of the FileCollection. 
      @throw InvalidStateException Thrown if the collection is invalid. */
    virtual std::string getName() const;

    /** Returns the number of entries in the FileCollection.
      @return the number of entries in the FileCollection. 
      @throw InvalidStateException Thrown if the collection is invalid. */
    virtual int size() const;

    /** The member function returns true if the collection is valid.
      @return true if the collection is valid.
     */ 
    bool isValid() const
    {
        return m_valid;
    }

    void mustBeValid() const;

    /** Create a heap allocated clone of the object this method is called for. The 
      caller is responsible for deallocating the clone when he is done with it.
      @return A heap allocated copy of the object this method is called for.
     */
    virtual FileCollection *    clone() const = 0;

protected:
    std::string                 m_filename;
    Entries                     m_entries;
    bool                        m_valid;
};





inline std::ostream & operator << (std::ostream& os, FileCollection const& collection)
{
    os << "collection '" << collection.getName() << "' {" ;
    ConstEntries entries(collection.entries());
    bool isFirst(true);
    for(auto it = entries.begin(); it != entries.end(); ++it)
    {
        if(!isFirst)
        {
            os << ", ";
            isFirst = false;
        }
        os << (*it)->getName();
    }
    os << "}";
    return os;
}


} // zipios namespace

// vim: ts=4 sw=4 et
