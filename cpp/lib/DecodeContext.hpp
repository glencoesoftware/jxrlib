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

#include "ImageMetadata.hpp"
#include "ImageDecoder.hpp"

namespace jxrlib {

  class DecodeContext {
  public:
    void decodeFrame(int frame,
                     ImageDecoder &source,
                     signed char *destination);

    signed char *decodeFrame(int frame,
                             std::string inputFile,
                             size_t *size);

    void decodeFrame(int frame,
                     std::string inputFile,
                     std::string outputFile);

    void decodeFrame(int frame,
                     std::string inputFile,
                     size_t offset,
                     unsigned char *destination);

    void decodeFrame(int frame,
                     unsigned char *source,
                     size_t sourceOffset,
                     size_t sourceLength,
                     unsigned char *destination,
                     size_t destinationOffset);

    signed char* decodeFrame(int frame,
                             char *source,
                             size_t offset,
                             size_t length,
                             size_t *size);

    ImageMetadata getImageMetadata(char *source,
                                   size_t offset,
                                   size_t length);

    ImageMetadata getImageMetadata(std::string inputFile,
                                   size_t offset);
  };

} // namespace jxrlib
