#ifndef DEFLATEOUTPUTSTREAMBUF_H
#define DEFLATEOUTPUTSTREAMBUF_H

#include "zipios++/zipios-config.h"

#include "zipios++/meta-iostreams.h"
#include <vector>

#include <zlib.h>

#include "zipios++/filteroutputstreambuf.h"

namespace zipios {

using std::vector ;

/** DeflateOutputStreambuf is an output stream filter, that deflates
    the data that is written to it before it passes it on to the
    output stream it is attached to. Deflation/Inflation is a
    compression/decompression method used in gzip and zip. The zlib
    library is used to perform the actual deflation, this class only
    wraps the functionality in an output stream filter. */
class DeflateOutputStreambuf : public FilterOutputStreambuf {
public:

  /** DeflateOutputStreambuf constructor.
      @param outbuf the streambuf to use for output.
      @param user_init If false user must invoke init() before writing any data. 
      (ZipOutputStreambuf needs to do this)
      @param del_outbuf if true is specified outbuf will be deleted, when 
      the DeflateOutputStreambuf is destructed. */
  explicit DeflateOutputStreambuf( streambuf *outbuf, bool user_init = false, 
				   bool del_outbuf = false ) ;

  /** Destructor. */
  virtual ~DeflateOutputStreambuf() ;

  bool init( int comp_level = 6 ) ;
  bool closeStream() ;

protected:
  virtual int overflow( int c = EOF ) ;
  virtual int sync() ;

  /** Flushes _outvec and updates _zs.next_out and _zs.avail_out. */
  bool flushOutvec() ;

  /** Flushes the remaining data in the zlib buffers, after which the
      only possible operations are deflateEnd() or deflateReset(). */
  void endDeflation() ;

private:
  z_stream _zs ;
  bool _zs_initialized ;
  const int _invecsize ;
  vector< char > _invec ;
protected: // FIXME: reconsider design?
  const int _outvecsize ;
  vector< char > _outvec ;

  /** If true dtor invokes endDeflation to flush the internal zlib
      buffers. Set to true in DeflateOutputStream ctors. A subclass
      can choose to set the flag to false and invoke endDeflation
      itself, if it requires it to be done at different times.  */
  bool _end_deflate ; // if true, dtor invokes endDeflation()


};


} // namespace



#endif

/** \file
    Header file that defines DeflateOutputStreambuf.
*/

/*
  Zipios++ - a small C++ library that provides easy access to .zip files.
  Copyright (C) 2000  Thomas Søndergaard
  
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
