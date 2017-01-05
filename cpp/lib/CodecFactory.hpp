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
#include <vector>

#include "FormatConverter.hpp"
#include "ImageDecoder.hpp"

#include "JXRGlue.h"

namespace jxrlib {

  /**
   *  The CodecFactory class wraps the `pCodecFactory` struct and corresponding
   *  methods from the jxrlib C code. It contains methods to generate image
   *  decoders and format converters.
   */
  class CodecFactory {
    PKCodecFactory *pCodecFactory;
    ERR err;
  public:
    CodecFactory();
    ~CodecFactory();

    /**
     *  Initialize an ImageDecoder from a file.
     *
     *  @param decoder   A pre-instantiated ImageDecoder instance to be initialized
     *  @param inputFile The JPEG XR file to be decoded
     */
    void decoderFromFile(ImageDecoder &decoder, std::string inputFile);

    /**
     *  Initialize an ImageDecoder from data inside a file.
     *
     *  @param decoder   A pre-instantiated ImageDecoder instance to be
     *                   initialized
     *  @param inputFile The file containing the JPEG XR data to be decoded
     *  @param offset    The offset within the file where image data begins
     */
    void decoderFromFile(ImageDecoder &decoder,
                         std::string inputFile,
                         long offset);

    /**
     *  Initialize an ImageDecoder directly from JPEG XR data
     *
     *  @param decoder A pre-instantiated ImageDecoder instance to be
     *                 initialized
     *  @param data    The raw JPEG XR data
     */
    void decoderFromBytes(ImageDecoder &decoder,
                          std::vector<unsigned char> data);

    /**
     *  Initialize an ImageDecoder directly from in-memory data containing a
     *  JPEG XR image
     *
     *  @param decoder A pre-instantiated ImageDecoder instance to be
     *                 initialized
     *  @param data    The data containing the JPEG XR image
     *  @param offset  Where in the `data` the JPEG XR image begins
     *  @param length  The length of the JPEG XR image data
     */
    void decoderFromBytes(ImageDecoder &decoder,
                          std::vector<unsigned char> data,
                          size_t offset,
                          size_t length);

    /**
     *  Initialize an ImageDecoder directly from in-memory data containing a
     *  JPEG XR image
     *
     *  @param decoder A pre-instantiated ImageDecoder instance to be
     *                 initialized
     *  @param bytes   A pointer to the location of the data containing the JPEG
     *                 XR image
     *  @param offset  The offset from `bytes` where the JPEG XR image begins
     *  @param length  The length of the JPEG XR image data
     */
    void decoderFromBytes(ImageDecoder &decoder,
                          unsigned char *bytes,
                          size_t offset,
                          size_t length);

    /**
     *  Generate a FormatConverter from a decoder which will convert a JPEG XR
     *  image into an image format specified by extension
     *
     *  @param decoder   An ImageDecoder initialized with a JPEG XR image
     *  @param extension The file extension of the image format to convert to
     *  @return          A FormatConverter ready to be used with an ImageEncoder
     *                   to transcode a JPEG XR image
     */
    FormatConverter createFormatConverter(ImageDecoder &decoder,
                                          std::string extension);
  };

} // namespace jxrlib
