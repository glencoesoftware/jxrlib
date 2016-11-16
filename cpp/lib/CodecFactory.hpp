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

  class CodecFactory {
    PKCodecFactory *pCodecFactory;
    ERR err;
  public:
    CodecFactory();
    ~CodecFactory();

    void decoderFromFile(ImageDecoder &decoder, std::string inputFile);
    void decoderFromFile(ImageDecoder &decoder,
                         std::string inputFile,
                         long offset);
    void decoderFromBytes(ImageDecoder &decoder,
                          std::vector<unsigned char> data);
    void decoderFromBytes(ImageDecoder &decoder,
                          std::vector<unsigned char> data,
                          size_t offset,
                          size_t length);
    void decoderFromBytes(ImageDecoder &decoder,
                          unsigned char *bytes,
                          size_t offset,
                          size_t length);
    void decoderFromVector(ImageDecoder &decoder,
                           std::vector<unsigned char> data);
    FormatConverter createFormatConverter(ImageDecoder &decoder,
                                          std::string extension);
  };

} // namespace jxrlib
