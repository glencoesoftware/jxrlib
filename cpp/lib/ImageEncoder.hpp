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

#include <string>

#include "windowsmediaphoto.h"
#include "FormatConverter.hpp"
#include "JXRGlue.h"
#include "Stream.hpp"
#include "ImageDecoder.hpp"

namespace jxrlib {

  /**
   *  The ImageEncoder class wraps the `pEncoder` struct and corresponding
   *  methods from the jxrlib C code. It contains methods to initialize the
   *  encoder given an ImageDecoder and to transcode between formats supported
   *  by the FormatConverter class.
   */
  class ImageEncoder {
    PKImageEncode *pEncoder;
    ERR err;
  public:

    /**
     *  Instantiates an ImageEncoder object for writing out to the provided
     *  Stream
     *
     *  @param encodeStream The Stream object encapsulating the destination for
     *                      the image to be written
     *  @param extension    A std::string containing the file extension
     *                      corresponding to the image format to use for encoding
     */
    ImageEncoder(Stream encodeStream, std::string extension);

    /**
     *  Initializes the underlying `pEncoder` struct using values obtained from
     *  the provided ImageDecoder
     *
     *  @param decoder The ImageDecoder from which to obtain image properties
     */
    void initializeWithDecoder(ImageDecoder& decoder);

    /**
     *  Encodes an image from and to the formats specified by the provided
     *  Format Converter
     *
     *  @param converter The FormatConverter object containing information about
     *                   which formats to convert from/to and the data to be
     *                   transcoded
     */
    void writeSource(FormatConverter& converter);

    /**
     *  Closes out the ImageEncoder and finalizes any pending writes.
     */
    void close();
  };

} // namespace jxrlib
