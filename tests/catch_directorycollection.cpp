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
 *
 * Zipios++ unit tests for the DirectoryEntry class.
 */

#include "catch_tests.h"

#include "zipios++/directorycollection.hpp"
#include "zipios++/zipiosexceptions.hpp"

#include "src/dostime.h"

#include <fstream>
#include <memory>
#include <vector>

#include <unistd.h>
#include <string.h>


namespace
{


char const g_letters[66]{
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 
    '.', '-', '_', '+'
};


} // no name namespace


/** \brief Class used to build a regular file.
 *
 * This file class creates a regular file in a directory.
 */
class file_t
{
public:
    typedef std::shared_ptr<file_t>     pointer_t;
    typedef std::vector<pointer_t>      vector_t;
    typedef std::vector<std::string>    filenames_t;

    /** \brief The type of file to create.
     *
     * The file_t() constructor expects one of those types to create
     * a new file. In most cases REGULAR is used.
     */
    enum class type_t
    {
        UNKNOWN,
        REGULAR,
        DIRECTORY
    };

    /** \brief Create a file.
     *
     * This function creates a file. If the creation fails, then an error
     * is thrown and the process attempts to clean up all the files
     * created so far.
     *
     * The children parameter is used whenever a DIRECTORY is created.
     * It determines the number of children that get created inside
     * that directory. Sub-directories (since the creation is recursive)
     * are given 1/3rd of that number. Also the function creates a new
     * sub-directory in about 1 in 10 files it creates.
     *
     * \param[in] t  The type of file (REGULAR or DIRECTORY).
     * \param[in] children_count  The number of children to create.
     */
    file_t(type_t t, int children_count, std::string const& new_filename = "")
        : m_filename(new_filename) // see below also
        , m_children() // see below
        , m_type(t)
    {
        // generate a random filename
        if(m_filename.empty())
        {
            for(;;)
            {
                size_t const l(rand() % 100 + 1);
                for(size_t idx(0); idx < l; ++idx)
                {
                    m_filename += g_letters[rand() % sizeof(g_letters)];
                }
                struct stat buf;
                if(m_filename != "inexistant" // very unlikely, but just in case...
                && stat(m_filename.c_str(), &buf) != 0)
                {
                    // file does not exist, return safely
                    break;
                }
            }
        }

        // This is only to test the validity of the exception handling
        //if(children_count < 20)
        //{
        //    throw std::logic_error("Ooops!");
        //}

        if(t == type_t::REGULAR)
        {
            // create a regular file
            // (the STL is expected to throw if the create fails from the constructor)
            std::ofstream os(m_filename, std::ios::out | std::ios::binary);
            size_t count(rand() % (100 * 1024 + 1)); // 0 to 100Kb
            for(size_t sz(0); sz < count; ++sz)
            {
                os << static_cast<unsigned char>(rand());
            }
            if(!os)
            {
                unlink(m_filename.c_str()); // LCOV_EXCL_LINE
                throw std::runtime_error("failed creating regular file"); // LCOV_EXCL_LINE
            }
        }
        else if(t == type_t::DIRECTORY)
        {
            if(mkdir(m_filename.c_str(), 0777) != 0)
            {
                throw std::runtime_error("failed creating directory"); // LCOV_EXCL_LINE
            }
            chdir(m_filename.c_str());
            for(int i(0); i < children_count; ++i)
            {
                try
                {
                    m_children.push_back(pointer_t(new file_t(rand() % 10 == 0 ? type_t::DIRECTORY : type_t::REGULAR, children_count / 3)));
                }
                catch(...)
                {
                    m_children.clear();
                    chdir("..");
                    rmdir(m_filename.c_str());
                    throw;
                }
            }
            chdir("..");
        }
        else
        {
            throw std::logic_error("unknown type of file"); // LCOV_EXCL_LINE
        }
    }

    /** \brief Clean up the file.
     *
     * This function ensures that this file or directory gets deleted
     * before deleting the object from memory.
     *
     * This function is recursive. When deleting a directory, all of
     * its children get deleted first.
     */
    ~file_t()
    {
        if(m_type == type_t::REGULAR)
        {
            unlink(m_filename.c_str());
        }
        else if(m_type == type_t::DIRECTORY)
        {
            // make sure to delete all the children first
            chdir(m_filename.c_str());
            m_children.clear();
            chdir("..");
            rmdir(m_filename.c_str());
        }
        else
        {
            // throw in destructor?!
            throw std::logic_error("unknown type of file"); // LCOV_EXCL_LINE
        }
    }

    /** \brief Retrieve the type of this file_t object.
     *
     * This function tells you whether this file_t object is a regular
     * file or a directory.
     *
     * \return REGULAR or DIRECTORY.
     */
    type_t type() const
    {
        return m_type;
    }

    /** \brief Return the filename.
     *
     * This function returns the filename of the file_t object.
     *
     * Since most filenames are generated, it is imperative to have a
     * way to retrieve the filename of a file_t.
     *
     * \note
     * Filenames are ASCII only (0-9, a-z, A-Z, and a few other characters.)
     *
     * \return The filename as a standard string.
     *
     * \sa g_letters
     */
    std::string const& filename() const
    {
        return m_filename;
    }

    /** \brief Retrieve the children of this file_t object.
     *
     * This function retrieves a vector of children. If the file is
     * a REGULAR file, then the list of children is always empty.
     *
     * To get the size of a certain directory, use children().size().
     */
    vector_t const& children() const
    {
        return m_children;
    }

    /** \brief Calculate the size of the tree starting at this file.
     *
     * This function is the total number of files this item represents,
     * including itself.
     *
     * The zip includes the directories since these are expected to
     * appear in the final Zip archive.
     *
     * \warning
     * This function returns a count that includes the root directory.
     * In other words, you have to use the result minus one to compare
     * with the total count of a DirectoryCollection.
     *
     * \return The total size.
     */
    size_t size()
    {
        size_t sz(1); // start with self
        for(size_t idx(0); idx < m_children.size(); ++idx)
        {
            sz += m_children[idx]->size();
        }
        return sz;
    }

    /** \brief Search a file in the tree.
     *
     * This function is used to search for a file in the tree. It is
     * rather slow, that being said, it is used to verify that we get
     * exactly the same list in the DirectoryCollection.
     *
     * \param[in] name  The fullname of the file to search.
     *
     * \return true if the file is found, false otherwise.
     */
    type_t find(std::string const& name)
    {
        std::string::size_type const pos(name.find('/'));

        std::string const segment(pos == std::string::npos ? name : name.substr(0, pos));

//std::cerr << "segment = [" << segment << "] vs filename [" << m_filename << "]\n";

        if(segment != m_filename)
        {
            // not a match...
            return type_t::UNKNOWN;
        }

        if(pos == std::string::npos)
        {
            // end of 'name' so we got a match
            return type();
        }

        std::string const remainder(name.substr(pos + 1));

        // this was a folder name, search for child
        for(auto it(m_children.begin()); it != m_children.end(); ++it)
        {
            type_t t((*it)->find(remainder));
            if(t != type_t::UNKNOWN)
            {
                return t;
            }
        }

        return type_t::UNKNOWN;
    }

    /** \brief Retrieve all the filenames.
     *
     * This function builds a vector of all the filenames defined in this
     * tree. The sub-folders get their path added as expected.
     *
     * \return An array with all the filenames defined in this tree.
     */
    filenames_t get_all_filenames() const
    {
        filenames_t names;
        get_filenames(names, m_filename);
        return names;
    }

private:
    void get_filenames(filenames_t& names, std::string const& parent) const
    {
        if(m_type == type_t::DIRECTORY)
        {
            // mark directories as such
            names.push_back(parent + "/");
        }
        else
        {
            names.push_back(parent);
        }
        for(auto it(m_children.begin()); it != m_children.end(); ++it)
        {
            file_t::pointer_t f(*it);
            std::string p(parent + "/" + f->filename());
            f->get_filenames(names, p);
        }
    }

    std::string     m_filename;
    vector_t        m_children;
    type_t          m_type;
};


SCENARIO("DirectoryCollection with invalid paths", "[DirectoryCollection] [FileCollection]")
{
    GIVEN("an empty directory collection")
    {
        zipios::DirectoryCollection dc;

        // first, check that the object is setup as expected
        SECTION("verify that the object looks as expected")
        {
            REQUIRE_FALSE(dc.isValid());
            REQUIRE_THROWS_AS(dc.entries().empty(), zipios::InvalidStateException);
            REQUIRE_THROWS_AS(dc.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
            REQUIRE_THROWS_AS(dc.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
            REQUIRE_THROWS_AS(dc.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
            REQUIRE_THROWS_AS(dc.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
            REQUIRE_THROWS_AS(dc.getName() == "-", zipios::InvalidStateException);   // default name is "-"
            REQUIRE_THROWS_AS(dc.size() == 0, zipios::InvalidStateException);
            REQUIRE_THROWS_AS(dc.mustBeValid(), zipios::InvalidStateException);

            zipios::DirectoryCollection copy_constructor(dc);
            REQUIRE_FALSE(copy_constructor.isValid());
            REQUIRE_THROWS_AS(copy_constructor.entries().empty(), zipios::InvalidStateException);
            REQUIRE_THROWS_AS(copy_constructor.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
            REQUIRE_THROWS_AS(copy_constructor.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
            REQUIRE_THROWS_AS(copy_constructor.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
            REQUIRE_THROWS_AS(copy_constructor.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
            REQUIRE_THROWS_AS(copy_constructor.getName() == "-", zipios::InvalidStateException);   // copy name as is
            REQUIRE_THROWS_AS(copy_constructor.size() == 0, zipios::InvalidStateException);
            REQUIRE_THROWS_AS(copy_constructor.mustBeValid(), zipios::InvalidStateException);

            zipios::DirectoryCollection copy_assignment;
            copy_assignment = dc;
            REQUIRE_FALSE(copy_assignment.isValid());
            REQUIRE_THROWS_AS(copy_assignment.entries().empty(), zipios::InvalidStateException);
            REQUIRE_THROWS_AS(copy_assignment.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
            REQUIRE_THROWS_AS(copy_assignment.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
            REQUIRE_THROWS_AS(copy_assignment.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
            REQUIRE_THROWS_AS(copy_assignment.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
            REQUIRE_THROWS_AS(copy_assignment.getName() == "-", zipios::InvalidStateException);   // copy name as is
            REQUIRE_THROWS_AS(copy_assignment.size() == 0, zipios::InvalidStateException);
            REQUIRE_THROWS_AS(copy_assignment.mustBeValid(), zipios::InvalidStateException);

            zipios::FileCollection::pointer_t clone(dc.clone());
            REQUIRE_FALSE(clone->isValid());
            REQUIRE_THROWS_AS(clone->entries().empty(), zipios::InvalidStateException);
            REQUIRE_THROWS_AS(clone->getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
            REQUIRE_THROWS_AS(clone->getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
            REQUIRE_THROWS_AS(clone->getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
            REQUIRE_THROWS_AS(clone->getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
            REQUIRE_THROWS_AS(clone->getName() == "-", zipios::InvalidStateException);   // copy name as is
            REQUIRE_THROWS_AS(clone->size() == 0, zipios::InvalidStateException);
            REQUIRE_THROWS_AS(clone->mustBeValid(), zipios::InvalidStateException);
        }

        WHEN("closing the directory")
        {
            dc.close();

            THEN("it is still the same")
            {
                REQUIRE_FALSE(dc.isValid());
                REQUIRE_THROWS_AS(dc.entries().empty(), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(dc.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(dc.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(dc.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(dc.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(dc.getName() == "-", zipios::InvalidStateException);   // default name is "-"
                REQUIRE_THROWS_AS(dc.size() == 0, zipios::InvalidStateException);
                REQUIRE_THROWS_AS(dc.mustBeValid(), zipios::InvalidStateException);

                zipios::DirectoryCollection copy_constructor(dc);
                REQUIRE_FALSE(copy_constructor.isValid());
                REQUIRE_THROWS_AS(copy_constructor.entries().empty(), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_constructor.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_constructor.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_constructor.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_constructor.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_constructor.getName() == "-", zipios::InvalidStateException);   // copy name as is
                REQUIRE_THROWS_AS(copy_constructor.size() == 0, zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_constructor.mustBeValid(), zipios::InvalidStateException);

                zipios::DirectoryCollection copy_assignment;
                copy_assignment = dc;
                REQUIRE_FALSE(copy_assignment.isValid());
                REQUIRE_THROWS_AS(copy_assignment.entries().empty(), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_assignment.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_assignment.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_assignment.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_assignment.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_assignment.getName() == "-", zipios::InvalidStateException);   // copy name as is
                REQUIRE_THROWS_AS(copy_assignment.size() == 0, zipios::InvalidStateException);
                REQUIRE_THROWS_AS(copy_assignment.mustBeValid(), zipios::InvalidStateException);

                zipios::FileCollection::pointer_t clone(dc.clone());
                REQUIRE_FALSE(clone->isValid());
                REQUIRE_THROWS_AS(clone->entries().empty(), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(clone->getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(clone->getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(clone->getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(clone->getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(clone->getName() == "-", zipios::InvalidStateException);   // copy name as is
                REQUIRE_THROWS_AS(clone->size() == 0, zipios::InvalidStateException);
                REQUIRE_THROWS_AS(clone->mustBeValid(), zipios::InvalidStateException);
            }
        }
    }
}


TEST_CASE("DirectoryCollection with a valid file, but not a directory", "[DirectoryCollection] [FileCollection]")
{
    // create a small random file
    int const file_size(rand() % 100 + 20);
    {
        // create a file
        std::ofstream f("directory-collection-test.txt", std::ios::out | std::ios::binary);
        for(int j(0); j < file_size; ++j)
        {
            char const c(rand());
            f << c;
        }
    }

    SECTION("verify that the object looks as expected in recursive mode")
    {
        // recursive reading
        zipios::DirectoryCollection dc("directory-collection-test.txt", true);

        // not valid because it is not a directory
        REQUIRE_FALSE(dc.isValid());
        REQUIRE_THROWS_AS(dc.entries().empty(), zipios::InvalidStateException);
        REQUIRE_THROWS_AS(dc.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
        REQUIRE_THROWS_AS(dc.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
        REQUIRE_THROWS_AS(dc.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
        REQUIRE_THROWS_AS(dc.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
        REQUIRE_THROWS_AS(dc.getName() == "directory-collection-test.txt", zipios::InvalidStateException);
        REQUIRE_THROWS_AS(dc.size() == 0, zipios::InvalidStateException);
        REQUIRE_THROWS_AS(dc.mustBeValid(), zipios::InvalidStateException);

        dc.close();

        // not valid because it is not a directory
        REQUIRE_FALSE(dc.isValid());
        REQUIRE_THROWS_AS(dc.entries().empty(), zipios::InvalidStateException);
        REQUIRE_THROWS_AS(dc.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
        REQUIRE_THROWS_AS(dc.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
        REQUIRE_THROWS_AS(dc.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
        REQUIRE_THROWS_AS(dc.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
        REQUIRE_THROWS_AS(dc.getName() == "directory-collection-test.txt", zipios::InvalidStateException);
        REQUIRE_THROWS_AS(dc.size() == 0, zipios::InvalidStateException);
        REQUIRE_THROWS_AS(dc.mustBeValid(), zipios::InvalidStateException);
    }

    SECTION("verify that the object looks as expected in non-recursive mode")
    {
        // recursive reading
        zipios::DirectoryCollection dc("directory-collection-test.txt", false);

        // not valid because it is not a directory
        REQUIRE_FALSE(dc.isValid());
        REQUIRE_THROWS_AS(dc.entries().empty(), zipios::InvalidStateException);
        REQUIRE_THROWS_AS(dc.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
        REQUIRE_THROWS_AS(dc.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
        REQUIRE_THROWS_AS(dc.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
        REQUIRE_THROWS_AS(dc.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
        REQUIRE_THROWS_AS(dc.getName() == "directory-collection-test.txt", zipios::InvalidStateException);
        REQUIRE_THROWS_AS(dc.size() == 0, zipios::InvalidStateException);
        REQUIRE_THROWS_AS(dc.mustBeValid(), zipios::InvalidStateException);

        dc.close();

        // not valid because it is not a directory
        REQUIRE_FALSE(dc.isValid());
        REQUIRE_THROWS_AS(dc.entries().empty(), zipios::InvalidStateException);
        REQUIRE_THROWS_AS(dc.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
        REQUIRE_THROWS_AS(dc.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
        REQUIRE_THROWS_AS(dc.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
        REQUIRE_THROWS_AS(dc.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
        REQUIRE_THROWS_AS(dc.getName() == "directory-collection-test.txt", zipios::InvalidStateException);
        REQUIRE_THROWS_AS(dc.size() == 0, zipios::InvalidStateException);
        REQUIRE_THROWS_AS(dc.mustBeValid(), zipios::InvalidStateException);
    }

    unlink("directory-collection-test.txt");
}


TEST_CASE("DirectoryCollection with valid trees of files", "[DirectoryCollection] [FileCollection]")
{
    for(int i(0); i < 6; ++i)
    {
        // create a directory tree starting in "tree"
        system("rm -rf tree"); // clean up, just in case
        size_t start_count(rand() % 40 + 80);
        file_t tree(file_t::type_t::DIRECTORY, start_count, "tree");

        {
            zipios::DirectoryCollection dc(zipios::DirectoryCollection("tree", true));

            SECTION("verify that the object looks as expected")
            {
                REQUIRE(dc.isValid());
                REQUIRE_FALSE(dc.entries().empty());
                REQUIRE_FALSE(dc.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH));
                REQUIRE_FALSE(dc.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE));
                REQUIRE_FALSE(dc.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH));
                REQUIRE_FALSE(dc.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE));
                REQUIRE(dc.getName() == "tree");
                REQUIRE(dc.size() == tree.size());
                dc.mustBeValid(); // not throwing

                {
                    zipios::FileEntry::vector_t v(dc.entries());
                    for(auto it(v.begin()); it != v.end(); ++it)
                    {
                        zipios::FileEntry::pointer_t entry(*it);

                        // verify that our tree knows about this file
                        file_t::type_t t(tree.find(entry->getName()));
                        REQUIRE(t != file_t::type_t::UNKNOWN);

                        struct stat file_stats;
                        REQUIRE(stat(entry->getName().c_str(), &file_stats) == 0);

                        REQUIRE((*it)->getComment().empty());
                        REQUIRE((*it)->getCompressedSize() == (*it)->getSize());
                        REQUIRE((*it)->getCrc() == 0);
                        REQUIRE((*it)->getEntryOffset() == 0);
                        REQUIRE((*it)->getExtra().empty());
                        REQUIRE((*it)->getHeaderSize() == 0);
                        REQUIRE((*it)->getMethod() == zipios::StorageMethod::STORED);
                        //REQUIRE((*it)->getName() == ...);
                        //REQUIRE((*it)->getFileName() == ...);
                        REQUIRE((*it)->getTime() == unix2dostime(file_stats.st_mtime));  // invalid date
                        REQUIRE((*it)->getUnixTime() == file_stats.st_mtime);
                        REQUIRE_FALSE((*it)->hasCrc());
                        if(t == file_t::type_t::DIRECTORY)
                        {
                            REQUIRE((*it)->isDirectory());
                            REQUIRE((*it)->getSize() == 0); // size is zero for directories
                        }
                        else
                        {
                            REQUIRE_FALSE((*it)->isDirectory());
                            REQUIRE((*it)->getSize() == file_stats.st_size);
                        }
                        REQUIRE((*it)->isValid());
                        //REQUIRE((*it)->toString() == "... (0 bytes)");

                        REQUIRE_THROWS_AS((*it)->read(std::cin), zipios::IOException);
                        REQUIRE_THROWS_AS((*it)->write(std::cout), zipios::IOException);
                    }
                }
            }

            SECTION("verify that the copy constructor works as expected")
            {
                zipios::DirectoryCollection copy_constructor(dc);
                REQUIRE(copy_constructor.isValid());
                REQUIRE_FALSE(copy_constructor.entries().empty());
                REQUIRE_FALSE(copy_constructor.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH));
                REQUIRE_FALSE(copy_constructor.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE));
                REQUIRE_FALSE(copy_constructor.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH));
                REQUIRE_FALSE(copy_constructor.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE));
                REQUIRE(copy_constructor.getName() == "tree");
                REQUIRE(copy_constructor.size() == tree.size());
                copy_constructor.mustBeValid();

                {
                    zipios::FileEntry::vector_t v(copy_constructor.entries());
                    for(auto it(v.begin()); it != v.end(); ++it)
                    {
                        zipios::FileEntry::pointer_t entry(*it);

                        // verify that our tree knows about this file
                        file_t::type_t t(tree.find(entry->getName()));
                        REQUIRE(t != file_t::type_t::UNKNOWN);

                        struct stat file_stats;
                        REQUIRE(stat(entry->getName().c_str(), &file_stats) == 0);

                        REQUIRE((*it)->getComment().empty());
                        REQUIRE((*it)->getCompressedSize() == (*it)->getSize());
                        REQUIRE((*it)->getCrc() == 0);
                        REQUIRE((*it)->getEntryOffset() == 0);
                        REQUIRE((*it)->getExtra().empty());
                        REQUIRE((*it)->getHeaderSize() == 0);
                        REQUIRE((*it)->getMethod() == zipios::StorageMethod::STORED);
                        //REQUIRE((*it)->getName() == ...);
                        //REQUIRE((*it)->getFileName() == ...);
                        REQUIRE((*it)->getTime() == unix2dostime(file_stats.st_mtime));  // invalid date
                        REQUIRE((*it)->getUnixTime() == file_stats.st_mtime);
                        REQUIRE_FALSE((*it)->hasCrc());
                        if(t == file_t::type_t::DIRECTORY)
                        {
                            REQUIRE((*it)->isDirectory());
                            REQUIRE((*it)->getSize() == 0); // size is zero for directories
                        }
                        else
                        {
                            REQUIRE_FALSE((*it)->isDirectory());
                            REQUIRE((*it)->getSize() == file_stats.st_size);
                        }
                        REQUIRE((*it)->isValid());
                        //REQUIRE((*it)->toString() == "... (0 bytes)");

                        REQUIRE_THROWS_AS((*it)->read(std::cin), zipios::IOException);
                        REQUIRE_THROWS_AS((*it)->write(std::cout), zipios::IOException);
                    }
                }
            }

            SECTION("verify that the copy assignment works as expected")
            {
                zipios::DirectoryCollection copy_assignment;
                copy_assignment = dc;
                REQUIRE(copy_assignment.isValid());
                REQUIRE_FALSE(copy_assignment.entries().empty());
                REQUIRE_FALSE(copy_assignment.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH));
                REQUIRE_FALSE(copy_assignment.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE));
                REQUIRE_FALSE(copy_assignment.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH));
                REQUIRE_FALSE(copy_assignment.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE));
                REQUIRE(copy_assignment.getName() == "tree");   // copy name as is
                REQUIRE(copy_assignment.size() == tree.size());
                copy_assignment.mustBeValid();

                {
                    zipios::FileEntry::vector_t v(copy_assignment.entries());
                    for(auto it(v.begin()); it != v.end(); ++it)
                    {
                        zipios::FileEntry::pointer_t entry(*it);

                        // verify that our tree knows about this file
                        file_t::type_t t(tree.find(entry->getName()));
                        REQUIRE(t != file_t::type_t::UNKNOWN);

                        struct stat file_stats;
                        REQUIRE(stat(entry->getName().c_str(), &file_stats) == 0);

                        REQUIRE((*it)->getComment().empty());
                        REQUIRE((*it)->getCompressedSize() == (*it)->getSize());
                        REQUIRE((*it)->getCrc() == 0);
                        REQUIRE((*it)->getEntryOffset() == 0);
                        REQUIRE((*it)->getExtra().empty());
                        REQUIRE((*it)->getHeaderSize() == 0);
                        REQUIRE((*it)->getMethod() == zipios::StorageMethod::STORED);
                        //REQUIRE((*it)->getName() == ...);
                        //REQUIRE((*it)->getFileName() == ...);
                        REQUIRE((*it)->getTime() == unix2dostime(file_stats.st_mtime));  // invalid date
                        REQUIRE((*it)->getUnixTime() == file_stats.st_mtime);
                        REQUIRE_FALSE((*it)->hasCrc());
                        if(t == file_t::type_t::DIRECTORY)
                        {
                            REQUIRE((*it)->isDirectory());
                            REQUIRE((*it)->getSize() == 0); // size is zero for directories
                        }
                        else
                        {
                            REQUIRE_FALSE((*it)->isDirectory());
                            REQUIRE((*it)->getSize() == file_stats.st_size);
                        }
                        REQUIRE((*it)->isValid());
                        //REQUIRE((*it)->toString() == "... (0 bytes)");

                        REQUIRE_THROWS_AS((*it)->read(std::cin), zipios::IOException);
                        REQUIRE_THROWS_AS((*it)->write(std::cout), zipios::IOException);
                    }
                }
            }

            SECTION("verify that clone() works as expected")
            {
                zipios::FileCollection::pointer_t clone(dc.clone());
                REQUIRE(clone->isValid());
                REQUIRE_FALSE(clone->entries().empty());
                REQUIRE_FALSE(clone->getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH));
                REQUIRE_FALSE(clone->getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE));
                REQUIRE_FALSE(clone->getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH));
                REQUIRE_FALSE(clone->getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE));
                REQUIRE(clone->getName() == "tree");
                REQUIRE(clone->size() == tree.size());
                clone->mustBeValid();

                {
                    zipios::FileEntry::vector_t v(clone->entries());
                    for(auto it(v.begin()); it != v.end(); ++it)
                    {
                        zipios::FileEntry::pointer_t entry(*it);

                        // verify that our tree knows about this file
                        file_t::type_t t(tree.find(entry->getName()));
                        REQUIRE(t != file_t::type_t::UNKNOWN);

                        struct stat file_stats;
                        REQUIRE(stat(entry->getName().c_str(), &file_stats) == 0);

                        REQUIRE((*it)->getComment().empty());
                        REQUIRE((*it)->getCompressedSize() == (*it)->getSize());
                        REQUIRE((*it)->getCrc() == 0);
                        REQUIRE((*it)->getEntryOffset() == 0);
                        REQUIRE((*it)->getExtra().empty());
                        REQUIRE((*it)->getHeaderSize() == 0);
                        REQUIRE((*it)->getMethod() == zipios::StorageMethod::STORED);
                        //REQUIRE((*it)->getName() == ...);
                        //REQUIRE((*it)->getFileName() == ...);
                        REQUIRE((*it)->getTime() == unix2dostime(file_stats.st_mtime));  // invalid date
                        REQUIRE((*it)->getUnixTime() == file_stats.st_mtime);
                        REQUIRE_FALSE((*it)->hasCrc());
                        if(t == file_t::type_t::DIRECTORY)
                        {
                            REQUIRE((*it)->isDirectory());
                            REQUIRE((*it)->getSize() == 0); // size is zero for directories
                        }
                        else
                        {
                            REQUIRE_FALSE((*it)->isDirectory());
                            REQUIRE((*it)->getSize() == file_stats.st_size);
                        }
                        REQUIRE((*it)->isValid());
                        //REQUIRE((*it)->toString() == "... (0 bytes)");

                        REQUIRE_THROWS_AS((*it)->read(std::cin), zipios::IOException);
                        REQUIRE_THROWS_AS((*it)->write(std::cout), zipios::IOException);
                    }
                }
            }

            SECTION("test the getInputStream() for each file")
            {
                // this one is recursive so we get ALL the files in
                // the collection
                file_t::filenames_t all_files(tree.get_all_filenames());

                for(auto it(all_files.begin()); it != all_files.end(); ++it)
                {
                    std::string const name(*it);

                    if(!name.empty() && name.back() == '/')  // Directory?
                    {
                        // directories cannot be attached to an istream
                        zipios::DirectoryCollection::stream_pointer_t is1(dc.getInputStream(name));
                        REQUIRE(!is1);

                        // also test with the ending '/', just in case
                        zipios::DirectoryCollection::stream_pointer_t is2(dc.getInputStream(name.substr(0, name.length() - 1)));
                        REQUIRE(!is2);

                        // now also test the getEntry() which works with MATCH
                        // or IGNORE -- prove it!
                        zipios::FileEntry::pointer_t entry_match(dc.getEntry(name.substr(0, name.length() - 1), zipios::FileCollection::MatchPath::MATCH));
                        REQUIRE(entry_match);

                        std::string::size_type pos(name.rfind('/', name.length() - 2));
                        if(pos == std::string::npos)
                        {
                            pos = 0;
                        }
                        else
                        {
                            ++pos;
                        }
                        zipios::FileEntry::pointer_t entry_ignore(dc.getEntry(name.substr(pos, name.length() - 1 - pos), zipios::FileCollection::MatchPath::IGNORE));
                        REQUIRE(entry_ignore);
                    }
                    else
                    {
                        // files must all work and we can read them and
                        // compare with the "real thing" and it is equal
                        zipios::DirectoryCollection::stream_pointer_t is(dc.getInputStream(name));
                        REQUIRE(is);

                        std::ifstream in(name, std::ios::in | std::ios::binary);

                        while(in && *is)
                        {
                            char buf1[BUFSIZ], buf2[BUFSIZ];

                            in.read(buf1, sizeof(buf1));
                            std::streamsize sz1(in.gcount());

                            is->read(buf2, sizeof(buf2));
                            std::streamsize sz2(is->gcount());

                            REQUIRE(sz1 == sz2);
                            REQUIRE(memcmp(buf1, buf2, sz1) == 0);
                        }

                        REQUIRE(!in);
                        REQUIRE(!*is);

                        // now also test the getEntry() which works with MATCH
                        // or IGNORE -- prove it!
                        zipios::FileEntry::pointer_t entry_match(dc.getEntry(name, zipios::FileCollection::MatchPath::MATCH));
                        REQUIRE(entry_match);

                        std::string::size_type pos(name.rfind('/'));
                        if(pos == std::string::npos)
                        {
                            pos = 0; // LCOV_EXCL_LINE
                        }
                        else
                        {
                            ++pos;
                        }
                        zipios::FileEntry::pointer_t entry_ignore(dc.getEntry(name.substr(pos, name.length() - pos), zipios::FileCollection::MatchPath::IGNORE));
                        REQUIRE(entry_ignore);
                    }
                }
            }

            SECTION("test the close() function for each file")
            {
                dc.close();

                // not valid because it was closed
                REQUIRE_FALSE(dc.isValid());
                REQUIRE_THROWS_AS(dc.entries().empty(), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(dc.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(dc.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(dc.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(dc.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(dc.getName() == "directory-collection-test.txt", zipios::InvalidStateException);
                REQUIRE_THROWS_AS(dc.size() == 0, zipios::InvalidStateException);
                REQUIRE_THROWS_AS(dc.mustBeValid(), zipios::InvalidStateException);
            }
        }

        {
            zipios::DirectoryCollection dc(zipios::DirectoryCollection("tree", false));

            SECTION("verify that the object looks as expected")
            {
                REQUIRE(dc.isValid());
                REQUIRE_FALSE(dc.entries().empty());
                REQUIRE_FALSE(dc.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH));
                REQUIRE_FALSE(dc.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE));
                REQUIRE_FALSE(dc.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH));
                REQUIRE_FALSE(dc.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE));
                REQUIRE(dc.getName() == "tree");
                REQUIRE(dc.size() == tree.children().size() + 1);
                dc.mustBeValid(); // not throwing

                {
                    zipios::FileEntry::vector_t v(dc.entries());
                    for(auto it(v.begin()); it != v.end(); ++it)
                    {
                        zipios::FileEntry::pointer_t entry(*it);

                        // verify that our tree knows about this file
                        file_t::type_t t(tree.find(entry->getName()));
                        REQUIRE(t != file_t::type_t::UNKNOWN);

                        struct stat file_stats;
                        REQUIRE(stat(entry->getName().c_str(), &file_stats) == 0);

                        REQUIRE((*it)->getComment().empty());
                        REQUIRE((*it)->getCompressedSize() == (*it)->getSize());
                        REQUIRE((*it)->getCrc() == 0);
                        REQUIRE((*it)->getEntryOffset() == 0);
                        REQUIRE((*it)->getExtra().empty());
                        REQUIRE((*it)->getHeaderSize() == 0);
                        REQUIRE((*it)->getMethod() == zipios::StorageMethod::STORED);
                        //REQUIRE((*it)->getName() == ...);
                        //REQUIRE((*it)->getFileName() == ...);
                        REQUIRE((*it)->getTime() == unix2dostime(file_stats.st_mtime));  // invalid date
                        REQUIRE((*it)->getUnixTime() == file_stats.st_mtime);
                        REQUIRE_FALSE((*it)->hasCrc());
                        if(t == file_t::type_t::DIRECTORY)
                        {
                            REQUIRE((*it)->isDirectory());
                            REQUIRE((*it)->getSize() == 0); // size is zero for directories
                        }
                        else
                        {
                            REQUIRE_FALSE((*it)->isDirectory());
                            REQUIRE((*it)->getSize() == file_stats.st_size);
                        }
                        REQUIRE((*it)->isValid());
                        //REQUIRE((*it)->toString() == "... (0 bytes)");

                        REQUIRE_THROWS_AS((*it)->read(std::cin), zipios::IOException);
                        REQUIRE_THROWS_AS((*it)->write(std::cout), zipios::IOException);
                    }
                }

                zipios::DirectoryCollection copy_constructor(dc);
                REQUIRE(copy_constructor.isValid());
                REQUIRE_FALSE(copy_constructor.entries().empty());
                REQUIRE_FALSE(copy_constructor.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH));
                REQUIRE_FALSE(copy_constructor.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE));
                REQUIRE_FALSE(copy_constructor.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH));
                REQUIRE_FALSE(copy_constructor.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE));
                REQUIRE(copy_constructor.getName() == "tree");
                REQUIRE(copy_constructor.size() == tree.children().size() + 1);
                copy_constructor.mustBeValid();

                {
                    zipios::FileEntry::vector_t v(copy_constructor.entries());
                    for(auto it(v.begin()); it != v.end(); ++it)
                    {
                        zipios::FileEntry::pointer_t entry(*it);

                        // verify that our tree knows about this file
                        file_t::type_t t(tree.find(entry->getName()));
                        REQUIRE(t != file_t::type_t::UNKNOWN);

                        struct stat file_stats;
                        REQUIRE(stat(entry->getName().c_str(), &file_stats) == 0);

                        REQUIRE((*it)->getComment().empty());
                        REQUIRE((*it)->getCompressedSize() == (*it)->getSize());
                        REQUIRE((*it)->getCrc() == 0);
                        REQUIRE((*it)->getEntryOffset() == 0);
                        REQUIRE((*it)->getExtra().empty());
                        REQUIRE((*it)->getHeaderSize() == 0);
                        REQUIRE((*it)->getMethod() == zipios::StorageMethod::STORED);
                        //REQUIRE((*it)->getName() == ...);
                        //REQUIRE((*it)->getFileName() == ...);
                        REQUIRE((*it)->getTime() == unix2dostime(file_stats.st_mtime));  // invalid date
                        REQUIRE((*it)->getUnixTime() == file_stats.st_mtime);
                        REQUIRE_FALSE((*it)->hasCrc());
                        if(t == file_t::type_t::DIRECTORY)
                        {
                            REQUIRE((*it)->isDirectory());
                            REQUIRE((*it)->getSize() == 0); // size is zero for directories
                        }
                        else
                        {
                            REQUIRE_FALSE((*it)->isDirectory());
                            REQUIRE((*it)->getSize() == file_stats.st_size);
                        }
                        REQUIRE((*it)->isValid());
                        //REQUIRE((*it)->toString() == "... (0 bytes)");

                        REQUIRE_THROWS_AS((*it)->read(std::cin), zipios::IOException);
                        REQUIRE_THROWS_AS((*it)->write(std::cout), zipios::IOException);
                    }
                }

                zipios::DirectoryCollection copy_assignment;
                copy_assignment = dc;
                REQUIRE(copy_assignment.isValid());
                REQUIRE_FALSE(copy_assignment.entries().empty());
                REQUIRE_FALSE(copy_assignment.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH));
                REQUIRE_FALSE(copy_assignment.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE));
                REQUIRE_FALSE(copy_assignment.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH));
                REQUIRE_FALSE(copy_assignment.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE));
                REQUIRE(copy_assignment.getName() == "tree");   // copy name as is
                REQUIRE(copy_assignment.size() == tree.children().size() + 1);
                copy_assignment.mustBeValid();

                {
                    zipios::FileEntry::vector_t v(copy_assignment.entries());
                    for(auto it(v.begin()); it != v.end(); ++it)
                    {
                        zipios::FileEntry::pointer_t entry(*it);

                        // verify that our tree knows about this file
                        file_t::type_t t(tree.find(entry->getName()));
                        REQUIRE(t != file_t::type_t::UNKNOWN);

                        struct stat file_stats;
                        REQUIRE(stat(entry->getName().c_str(), &file_stats) == 0);

                        REQUIRE((*it)->getComment().empty());
                        REQUIRE((*it)->getCompressedSize() == (*it)->getSize());
                        REQUIRE((*it)->getCrc() == 0);
                        REQUIRE((*it)->getEntryOffset() == 0);
                        REQUIRE((*it)->getExtra().empty());
                        REQUIRE((*it)->getHeaderSize() == 0);
                        REQUIRE((*it)->getMethod() == zipios::StorageMethod::STORED);
                        //REQUIRE((*it)->getName() == ...);
                        //REQUIRE((*it)->getFileName() == ...);
                        REQUIRE((*it)->getTime() == unix2dostime(file_stats.st_mtime));  // invalid date
                        REQUIRE((*it)->getUnixTime() == file_stats.st_mtime);
                        REQUIRE_FALSE((*it)->hasCrc());
                        if(t == file_t::type_t::DIRECTORY)
                        {
                            REQUIRE((*it)->isDirectory());
                            REQUIRE((*it)->getSize() == 0); // size is zero for directories
                        }
                        else
                        {
                            REQUIRE_FALSE((*it)->isDirectory());
                            REQUIRE((*it)->getSize() == file_stats.st_size);
                        }
                        REQUIRE((*it)->isValid());
                        //REQUIRE((*it)->toString() == "... (0 bytes)");

                        REQUIRE_THROWS_AS((*it)->read(std::cin), zipios::IOException);
                        REQUIRE_THROWS_AS((*it)->write(std::cout), zipios::IOException);
                    }
                }

                zipios::FileCollection::pointer_t clone(dc.clone());
                REQUIRE(clone->isValid());
                REQUIRE_FALSE(clone->entries().empty());
                REQUIRE_FALSE(clone->getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH));
                REQUIRE_FALSE(clone->getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE));
                REQUIRE_FALSE(clone->getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH));
                REQUIRE_FALSE(clone->getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE));
                REQUIRE(clone->getName() == "tree");
                REQUIRE(clone->size() == tree.children().size() + 1);
                clone->mustBeValid();

                {
                    zipios::FileEntry::vector_t v(clone->entries());
                    for(auto it(v.begin()); it != v.end(); ++it)
                    {
                        zipios::FileEntry::pointer_t entry(*it);

                        // verify that our tree knows about this file
                        file_t::type_t t(tree.find(entry->getName()));
                        REQUIRE(t != file_t::type_t::UNKNOWN);

                        struct stat file_stats;
                        REQUIRE(stat(entry->getName().c_str(), &file_stats) == 0);

                        REQUIRE((*it)->getComment().empty());
                        REQUIRE((*it)->getCompressedSize() == (*it)->getSize());
                        REQUIRE((*it)->getCrc() == 0);
                        REQUIRE((*it)->getEntryOffset() == 0);
                        REQUIRE((*it)->getExtra().empty());
                        REQUIRE((*it)->getHeaderSize() == 0);
                        REQUIRE((*it)->getMethod() == zipios::StorageMethod::STORED);
                        //REQUIRE((*it)->getName() == ...);
                        //REQUIRE((*it)->getFileName() == ...);
                        REQUIRE((*it)->getTime() == unix2dostime(file_stats.st_mtime));  // invalid date
                        REQUIRE((*it)->getUnixTime() == file_stats.st_mtime);
                        REQUIRE_FALSE((*it)->hasCrc());
                        if(t == file_t::type_t::DIRECTORY)
                        {
                            REQUIRE((*it)->isDirectory());
                            REQUIRE((*it)->getSize() == 0); // size is zero for directories
                        }
                        else
                        {
                            REQUIRE_FALSE((*it)->isDirectory());
                            REQUIRE((*it)->getSize() == file_stats.st_size);
                        }
                        REQUIRE((*it)->isValid());
                        //REQUIRE((*it)->toString() == "... (0 bytes)");

                        REQUIRE_THROWS_AS((*it)->read(std::cin), zipios::IOException);
                        REQUIRE_THROWS_AS((*it)->write(std::cout), zipios::IOException);
                    }
                }
            }

            SECTION("test the getInputStream() for each file")
            {
                // in this case the DirectoryCollection is not recursive
                // so only the top children are available
                file_t::vector_t all_files(tree.children());

                for(auto it(all_files.begin()); it != all_files.end(); ++it)
                {
                    file_t::pointer_t f(*it);

                    if(f->type() == file_t::type_t::DIRECTORY)  // Directory?
                    {
                        // directories cannot be attached to an istream
                        zipios::DirectoryCollection::stream_pointer_t is1(dc.getInputStream(f->filename()));
                        REQUIRE(!is1);
                    }
                    else
                    {
                        // WARNING: in this case we get a "bare" filename
                        //          from the file_t object
                        //
                        // files must all work and we can read them and
                        // compare with the "real thing" and it is equal

                        // "tree/" missing...
                        zipios::DirectoryCollection::stream_pointer_t bad_is(dc.getInputStream(f->filename(), zipios::FileCollection::MatchPath::MATCH));
                        REQUIRE_FALSE(bad_is);

                        // in this case we ignore to see that we can indeed
                        // get the file... unfortunately it could be another
                        // file with the same name (although unlikely, it
                        // is very much possible)
                        zipios::DirectoryCollection::stream_pointer_t is(dc.getInputStream(f->filename(), zipios::FileCollection::MatchPath::IGNORE));
                        REQUIRE(is);

                        // So here we verify that it find the correct
                        // entry, if so then we can compare the files
                        zipios::FileEntry::pointer_t entry_match(dc.getEntry("tree/" + f->filename(), zipios::FileCollection::MatchPath::MATCH));
                        zipios::FileEntry::pointer_t entry_ignore(dc.getEntry(f->filename(), zipios::FileCollection::MatchPath::IGNORE));
                        if(entry_match == entry_ignore)
                        {
                            std::ifstream in("tree/" + f->filename(), std::ios::in | std::ios::binary);

                            while(in && *is)
                            {
                                char buf1[BUFSIZ], buf2[BUFSIZ];

                                in.read(buf1, sizeof(buf1));
                                std::streamsize sz1(in.gcount());

                                is->read(buf2, sizeof(buf2));
                                std::streamsize sz2(is->gcount());

                                REQUIRE(sz1 == sz2);
                                REQUIRE(memcmp(buf1, buf2, sz1) == 0);
                            }

                            REQUIRE(!in);
                            REQUIRE(!*is);
                        }
                    }
                }
            }

            SECTION("verify that the object looks invalid after a close")
            {
                dc.close();

                // not valid because it is not a directory
                REQUIRE_FALSE(dc.isValid());
                REQUIRE_THROWS_AS(dc.entries().empty(), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(dc.getEntry("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(dc.getEntry("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(dc.getInputStream("inexistant", zipios::FileCollection::MatchPath::MATCH), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(dc.getInputStream("inexistant", zipios::FileCollection::MatchPath::IGNORE), zipios::InvalidStateException);
                REQUIRE_THROWS_AS(dc.getName() == "directory-collection-test.txt", zipios::InvalidStateException);
                REQUIRE_THROWS_AS(dc.size() == 0, zipios::InvalidStateException);
                REQUIRE_THROWS_AS(dc.mustBeValid(), zipios::InvalidStateException);
            }
        }
    }
}


// vim: ts=4 sw=4 et
