/*
  Zipios -- a small C++ library that provides easy access to .zip files.

  Copyright (C) 2000-2007  Thomas Sondergaard
  Copyright (C) 2015-2019  Made to Order Software Corporation

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

/** \file
 *
 * Zipios unit tests for the DirectoryCollection class.
 */

#include "tests.hpp"

#include <fstream>
//
#include <unistd.h>
#include <sys/stat.h>


namespace zipios_test
{


namespace
{


char const g_letters[66]{
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    '.', '-', '_', '+'
};


} // no name namespace


/** \class file_t
 * \brief Class used to build a regular file.
 *
 * This file class creates a regular file in a directory.
 */


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
file_t::file_t(type_t t, int children_count, std::string const& new_filename)
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
        } // LCOV_EXCL_LINE
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
file_t::~file_t()
{
// use this return to keep the tree to check files before they get deleted
//return;
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
        // we cannot throw in a destructor... (g++ 7.0+ detects that now!)
        //throw std::logic_error("unknown type of file"); // LCOV_EXCL_LINE
        std::cerr << "fatal error: unknown type of file" << std::endl; // LCOV_EXCL_LINE
        std::terminate(); // LCOV_EXCL_LINE
    }
}

/** \brief Retrieve the type of this file_t object.
 *
 * This function tells you whether this file_t object is a regular
 * file or a directory.
 *
 * \return REGULAR or DIRECTORY.
 */
file_t::type_t file_t::type() const
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
std::string const& file_t::filename() const
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
file_t::vector_t const& file_t::children() const
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
size_t file_t::size()
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
file_t::type_t file_t::find(std::string const& name)
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
file_t::filenames_t file_t::get_all_filenames() const
{
    filenames_t names;
    get_filenames(names, m_filename);
    return names;
}

void file_t::get_filenames(filenames_t& names, std::string const& parent) const
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


} // zipios_tests namespace

// Local Variables:
// mode: cpp
// indent-tabs-mode: nil
// c-basic-offset: 4
// tab-width: 4
// End:

// vim: ts=4 sw=4 et
