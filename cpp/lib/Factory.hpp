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

#ifndef _Factory
#define _Factory

#include <string>

#include "JXRGlue.h"
#include "Stream.hpp"

namespace jxrlib {

  /**
   *  The Factory class wraps the `pFactory` struct and corresponding methods
   *  from the jxrlib C code. It contains methods to generate streams from image
   *  files.
   */
  class Factory {
    PKFactory *pFactory;
    ERR err;
  public:
    Factory();
    ~Factory();

    /**
     *  Create a Stream, useful for initializing an ImageDecoder, from a file.
     *
     *  @param filename Path to the file which should be converted to a Stream
     *  @return A Stream object encapsulating the data from the provided file
     */
    Stream createStreamFromFilename(std::string filename);
  };

} // namespace jxrlib

#endif // _Factory
