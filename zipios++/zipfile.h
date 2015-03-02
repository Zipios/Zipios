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
    Header file that defines ZipFile.
*/

/** \mainpage Zipios++
 *
 * \image html   zipios++.jpg
 *
 * \section intro Introduction
 *
 * Zipios++ is a java.util.zip-like C++ library for reading and
 * writing Zip files. Access to individual entries is provided through
 * standard C++ iostreams. A simple read-only virtual file system that
 * mounts regular directories and zip files is also provided.
 *
 * The source code is released under the <a
 * href="http://www.gnu.org/copyleft/lesser.html">GNU Lesser General Public
 * License</a>.
 *
 * \section status Status
 *
 * Spanned archives are not supported, and support is not planned.
 *
 * The library has been tested and appears to be working with
 * <ul>
 * <li><a href="http://www.freebsd.org/ports/archivers.html#zipios++-0.1.5">FreeBSD stable and current / gcc 2.95.3</a></li>
 * <li>Red Hat Linux release 7.0  / gcc 2.96</li>
 * <li>Red Hat Linux release 6.2 (Zoot) / egcs-2.91.66</li>
 * <li>Linux Mandrake release 7.0 (Air) / gcc 2.95.2</li>
 * <li>SGI IRIX64 6.5 / gcc 2.95.2</li>
 * <li>SGI IRIX64 6.5 / MIPSpro Compilers: Version 7.30</li>
 * </ul>
 *
 * If you are aware of any other platforms that Zipios++ works on,
 * please let me know (thomass@deltadata.dk).
 *
 * \section documentation Documentation 
 * This web page is the front page to the library documentation which
 * is generated from the source files using <a
 * href="http://www.stack.nl/~dimitri/doxygen/index.html">Doxygen</a>. Use
 * the links at the top of the page to browse the API
 * documentation. The documentation is also available in
 * a printer-friendly format <a href="refman.pdf">[pdf]</a>.
 *
 * \subsection zipfiles Zip file access
 * The two most important classes are \ref zipfile_anchor "ZipFile" and 
 * \ref ZipInputStream_anchor "ZipInputStream". ZipInputStream is an istream
 * for reading zipfiles. It can be instantiated directly, without the
 * use of ZipFile. \ref ZipInputStream_getnextentry_anchor 
 * "ZipInputStream::getNextEntry()" positions the new ZipInputStream at the
 * first entry. The following entry can be accessed by calling
 * \ref ZipInputStream_getnextentry_anchor "ZipInputStream::getNextEntry()"
 * again.
 *
 * ZipFile scans the central directory of a zipfile and provides an
 * interface to access that directory. The user may search for entries
 * with a particular filename using \ref fcoll_getentry_anchor "ZipFile::getEntry()", 
 * or simply get the complete list of entries
 * with \ref fcoll_entries_anchor "ZipFile::entries()". To get an
 * istream (ZipInputStream) to a particular entry simply use
 * \ref fcoll_getinputstream "ZipFile::getInputStream()".
 *
 * \ref example_zip_anchor "example_zip.cpp" demonstrates the central
 * elements of Zipios++.
 *
 * A Zip file appended to another file, e.g. a binary program, with the program
 * \ref appendzip_anchor "appendzip", can be read with 
 * \ref zipfile_openembeddedzipfile "ZipFile::openEmbeddedZipFile()".
 *
 * \subsection filecollections FileCollections
 *
 * A ZipFile is actually just a special kind of 
 * \ref fcoll_anchor "FileCollection" that
 * obtains its entries from a .zip Zip archive. Zipios++ also implements
 * a \ref dircol_anchor "DirectoryCollection" that obtains its entries 
 * from a specified directory, and a \ref collcoll_anchor "CollectionCollection" 
 * that obtains its entries from
 * other collections. Using a single CollectionCollection any number of
 * other FileCollections can be placed under its control and accessed
 * through the same single interface that is used to access a ZipFile or
 * a DirectoryCollection. A singleton (a unique global instance)
 * CollectionCollection can be obtained through
 *
 * \ref collcoll_inst_anchor "CollectionCollection::inst()" ;
 *
 * To save typing CollectionCollection has been typedef'ed to CColl. In
 * the initialization part of an application FileCollections can be
 * created, and placed under CColll::inst()'s control using
 *
 * \ref collcoll_addcoll_anchor "CColl::inst().addCollection()"
 *
 * and later an istream can be obtained using
 *
 * \ref fcoll_getinputstream "CColl::inst().getInputStream()".
 *
 * \section download Download 
 * Go to Zipios++ project page on SourceForge for tar balls and ChangeLog.
 * <a href="http://sourceforge.net/project/?group_id=5418" >
 * http://sourceforge.net/project/?group_id=5418</a>
 *
 * \section links Links
 * <a href="ftp://ftp.freesoftware.com/pub/infozip/zlib/zlib.html">zlib</a>. 
 * The compression library that Zipios++ uses to perform the actual 
 * decompression.
 *
 * <a href="http://java.sun.com/products/jdk/1.3/docs/api/index.html">
 * Java 2 Platform, Standard Edition, v 1.3 API Specification
 * </a>. Zipios++ is heavily inspired by the java.util.zip package.
 *
 * <a
 * href="http://www.geocities.com/SiliconValley/Lakes/2160/fformats/files/zip.txt">
 * PKWARE zip format 
 * </a>. A description of the zip file format.
 *
 * \section bugs Bugs 
 * Submit bug reports and patches to thomass@deltadata.dk 
 *
 * \htmlonly
 * Project hosted by <a href="http://sourceforge.net">
 * <img src="http://sourceforge.net/sflogo.php?group_id=5418&type=1" >
 * </a><p>
 * Logo created with <a href="http://www.webgfx.ch/titlepic.htm">
 * <img src="webgfx.gif" >
 * </a>
 * \endhtmlonly
 */

#include "zipios++/fcoll.h"
#include "zipios++/ziphead.h"
#include "zipios++/virtualseeker.h"

namespace zipios
{


/** \anchor zipfile_anchor
 *  ZipFile is a FileCollection, where the files are stored
 * in a .zip file.
 */
class ZipFile : public FileCollection
{
public:
  /** \anchor zipfile_openembeddedzipfile
   *  Opens a Zip archive embedded in another file, by writing the zip
   *  archive to the end of the file followed by the start offset of
   *  the zip file. The offset must be written in zip-file byte-order
   *  (little endian). The program appendzip, which is part of the
   *  Zipios++ distribution can be used to append a Zip archive to a
   *  file, e.g. a binary program. 
   *  @throw FColException Thrown if the specified file name is not a valid zip 
   *  archive.
   *  @throw IOException Thrown if an I/O problem is encountered, while the directory
   *  of the specified zip archive is being read.
   */
  static ZipFile openEmbeddedZipFile( std::string const& name ) ;

  /** Default constructor.
   */
  ZipFile() {}

  /* Default Copy constructor and copy assignment operator are sufficient. */

  /** Constructor. Opens the zip file name. If the zip "file" is
   *  embedded in a file that contains other data, e.g. a binary
   *  program, the offset of the zip file start and end must be
   *  specified. 
   *  @param name The filename of the zip file to open.
   *  @param s_off Offset relative to the start of the file, that 
   *  indicates the beginning of the zip file.
   *  @param e_off Offset relative to the end of the file, that
   *  indicates the end of the zip file. The offset is a positive number,
   *  even though the offset is towards the beginning of the file.
   *  @throw FColException Thrown if the specified file name is not a valid zip 
   *  archive.
   *  @throw IOException Thrown if an I/O problem is encountered, while the directory
   *  of the specified zip archive is being read.
   */
  explicit ZipFile( std::string const& name, int s_off = 0, int e_off = 0 ) ;

  virtual FileCollection *clone() const ;

  /** Destructor. */
  virtual ~ZipFile() ;

  virtual void close() ;

  virtual std::istream *getInputStream( ConstEntryPointer const& entry ) ;
  virtual std::istream *getInputStream( std::string const& entry_name, MatchPath matchpath = MatchPath::MATCH ) ;

private:
  VirtualSeeker         _vs ;
  EndOfCentralDirectory _eocd ;

  bool init( std::istream& _zipfile ) ;
  bool readCentralDirectory( std::istream& _zipfile ) ;
  bool readEndOfCentralDirectory( std::istream& _zipfile ) ;
  bool confirmLocalHeaders( std::istream& _zipfile ) ;
};


} // zipios namespace

// vim: ts=2 sw=2 et
