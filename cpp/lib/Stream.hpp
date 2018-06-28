/*
 * #%L
 * Copyright (C) 2016 Glencoe Software, Inc. All rights reserved.
 * %%
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 * #%L
 */
#pragma once

#include <vector>

#include "JXRGlue.h"
#include "windowsmediaphoto.h"

namespace jxrlib {

  /**
   *  The Stream struct holds a reference to the `pStream` C struct from the
   *  jxrlib C library. It has the ability to be instantiated from random data
   *  in memory, as well as to be used by other methods (e.g. to open a file).
   */
  struct Stream {
    struct WMPStream *pStream;
    ERR err;
    Stream() : pStream(NULL), err(WMP_errSuccess) {}
    // No destructor required as resources are cleaned up by
    // PKImageDecode_Release
    //~Stream();

    /**
     *  Instantiates a Stream backed by data, of a specified length, in memory
     *
     *  @param bytes The location of the beginning of the in memory data
     *  @param len   The length in bytes of the memory to treat as a stream
     */
    Stream(unsigned char *bytes, size_t len);
  };

} // namespace jxrlib
