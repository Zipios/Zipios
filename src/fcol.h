#ifndef FCOL_H
#define FCOL_H

#include "config.h"

#include <vector>
#include <string>

#include "fcolexceptions.h"
#include "fileentry.h"

namespace fcol {


/** \anchor fcol_anchor
    FileCollection is an abstract baseclass, that represents a
    collection of files. The specializations of FileCollection
    represents different origins of file collections, such as
    directories, simple filename lists and compressed archives. */
class FileCollection {
public:
  /** FileCollection constructor. */
  explicit FileCollection() 
    : _filename( "-"   ),
      _entries ( 0     ),
      _valid   ( false ) {}

  /** Closes the FileCollection. */
  virtual void close() = 0 ;
  /** \anchor fcol_entries_anchor
      Returns a vector of const pointers to the entries in the
      FileCollection.  
      @return a vector< ConstEntryPointer >
      containing the entries of the FileCollection. */
  virtual vector< ConstEntryPointer > entries() const ;

  enum MatchPath { IGNORE, MATCH } ;

  /** \anchor fcol_getentry_anchor
      Returns a ConstEntryPointer to a FileEntry object for the entry 
      with the specified name. To ignore the path part of the filename in search of a
      match, specify FileCollection::IGNORE as the second argument.
      @param name A string containing the name of the entry to get.
      @param matchpath Speficy MATCH, if the path should match as well, 
      specify IGNORE, if the path should be ignored.
      @return A ConstEntryPointer to the found entry. The returned pointer
      equals zero if no entry is found.
  */
  virtual ConstEntryPointer getEntry( const string &name, 
				     MatchPath matchpath = MATCH ) const ;
  /** \anchor fcol_getinputstream
      Returns a pointer to an opened istream for the specified
      FileEntry. It is the callers responsibility to delete the stream
      when he is done with it. Returns 0, if there is no such
      FileEntry in the FileCollection.
      @param entry A ConstEntryPointer to the FileEntry to get an istream to.
      @return an open istream for the specified entry. The istream is allocated on
      heap and it is the users responsibility to delete it when he is done with it.
  */
  virtual istream *getInputStream( const ConstEntryPointer &entry ) = 0 ;
  /** Returns a pointer to an opened istream for the specified
      entry name. It is the callers responsibility to delete the stream
      when he is done with it. Returns 0, if there is no entry with the specified
      name in the FileCollection.
      @param matchpath Speficy MATCH, if the path should match as well, 
      specify IGNORE, if the path should be ignored.
      @return an open istream for the specified entry. The istream is allocated on
      heap and it is the users responsibility to delete it when he is done with it.
  */
  virtual istream *getInputStream( const string &entry_name, 
				     MatchPath matchpath = MATCH ) = 0 ;
  /** Returns the name of the FileCollection.
      @return the name of the FileCollection. */
  virtual string getName() const ;
  /** Returns the number of entries in the FileCollection.
      @return the number of entries in the FileCollection. 
  */
  virtual int size() const ;
  /** FileCollection destructor. */
  virtual ~FileCollection () ;
protected:
  string _filename ;
  vector< EntryPointer > _entries ;
  bool _valid ;
};

}

#endif


/**
   \mainpage Zipios++

   \image html   zipios++.png
   \image latex  zipios++.eps width=10cm
   
   \section intro Introduction
   
   Zipios++ is a small C++ library for reading zip files. The structure
   and public interface are based (somewhat loosely) on the java.util.zip
   package. The streams created to access the individual entries in a zip
   file are based on the standard iostream library.
   
   Zipios++ also provides a way for an application to support files from
   multiple sources (e.g. from zip files or from ordinary directories)
   transparently.
   
   The source code is released under the <A
   HREF="http://www.gnu.org/copyleft/gpl.html>GNU General Public
   License</A>.
   
   \section status Status
   Writing is not supported yet
   
   Spanned archives are not supported, and support is not planned.
   
   The library is only tested under RedHat 6.2 linux and SGI Irix 6.5. 
   
   \section documentation Documentation 
   This web page is the front page to the library documentation, that
   is generated from the source files using <A
   HREF="http://www.stack.nl/~dimitri/doxygen/index.html">Doxygen</A>. Use
   the links at the top of the page to browse the API
   documentation. The documentation is also available in
   printer-friendly formats <A HREF="refman.pdf">[pdf]</A>, <A
   HREF="refman.ps">[ps]</A>.
   
   \subsection zipfiles Zip file access
   The two most important classes are \ref zipfile_anchor "ZipFile" and 
   \ref ZipInputStream_anchor "ZipInputStream". ZipInputStream is an istream
   for reading zipfiles. It can be instantiated directly, without the
   use of ZipFile. A new ZipInputStream reads from the first entry, and
   the user can skip to the next entry by calling
   \ref ZipInputStream_getnextentry_anchor "ZipInputStream::getNextEntry()".
   
   ZipFile scans the central directory of a zipfile and provides an
   interface to access that directory. The user may search for entries
   with a particular filename using \ref fcol_getentry_anchor "ZipFile::getEntry()", 
   or simply get the complete list of entries
   with \ref fcol_entries_anchor "ZipFile::entries()". To get an
   istream (ZipInputStream) to a particular entry simply use
   \ref fcol_getinputstream "ZipFile::getInputStream()".
   
   \ref example_zip_anchor "example_zip.cpp" demonstrates the central
   elements of Zipios++.
   
   \subsection filecollections FileCollections
   
   A ZipFile is actually just a special kind of 
   \ref fcol_anchor "FileCollection" that
   obtains its entries from a .zip Zip archive. Zipios++ also implements
   a \ref dircol_anchor "DirectoryCollection" that obtains its entries 
   from a specified directory, and a \ref collcoll_anchor "CollectionCollection" 
   that obtains its entries from
   other collections. Using a single CollectionCollection any number of
   other FileCollections can be placed under its control and accessed
   through the same single interface that is used to access a ZipFile or
   a DirectoryCollection. A singleton (a unique global instance)
   CollectionCollection can be obtained through
   
   \ref collcoll_inst_anchor "CollectionCollection::inst()" ;

   To save typing CollectionCollection has been typedef'ed to CCol. In
   the initialization part of an application FileCollections can be
   created, and placed under CColl::inst()'s control using
   
   \ref collcoll_addcoll_anchor "CCol::inst()->addCollection()"
   
   and later an istream can be obtained using

   \ref fcol_getinputstream "CCol::inst()->getInputStream()".
   
   \section download Download 
   Go to Zipios++ project page on SourceForge.
   <A HREF="http://sourceforge.net/project/?group_id=5418" >
   http://sourceforge.net/project/?group_id=5418</A>

   
   \htmlonly
   <A HREF="ChangeLog">ChangeLog</A>
   \endhtmlonly
   
   \section links Links
   <A HREF="http://www.miba.auc.dk/~thomas/streambuf">Deriving
   the C++ iostreams and streambuf</A>
   
   <A HREF="http://www.fensende.com/zpp/">Zpp</A>. A library very similar 
   to Zipios++.
   
   <A HREF="http://www.ajk.tele.fi/~too/sw/zip08x.readme">zip08x</A>.
   A library very similar to Zipios++, but for C.
   
   <A HREF="http://artpacks.acid.org/pub/infozip/zlib/zlib.html">zlib</A>. 
   The compression library that Zipios++ uses to perform the actual 
   decompression.
   
   <A HREF="http://java.sun.com/products/jdk/1.3/docs/api/index.html">
   Java 2 Platform, Standard Edition, v 1.3 API Specification
   </A>. Zipios++ is heavily inspired by the java.util.zip package.
   
   <A
   HREF="http://www.geocities.com/SiliconValley/Lakes/2160/fformats/files/zip.txt">
   PKWARE zip format 
   </A>. A description of the zip file format.
   
   \section bugs Bugs 
   Submit bug reports and patches to thomas@miba.auc.dk 
   
   
   
   \htmlonly
   Project hosted by <A HREF="http://sourceforge.net">
   <img src="http://sourceforge.net/sflogo.php?group_id=5418&type=1" >
   </A><p>
   Logo created with <A HREF="http://www.webgfx.ch/titlepic.htm">
   <img src="webgfx.gif" >
   </A>
   \endhtmlonly
*/


/** \file
    Header file that defines FileCollection.
*/

/*
  Zipios++ - a small C++ library that provides easy access to .zip files.
  Copyright (C) 2000  Thomas Søndergaard
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
