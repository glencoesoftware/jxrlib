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

#include "Resolution.hpp"
#include <cstdlib>

namespace jxrlib {

  /**
   *  The ImageMetadata struct holds all relevant information for a JPEG-XR
   *  encoded image.
   */
  struct ImageMetadata {

    /**
     *  A Resolution object containing information about the encoded image's
     *  Resolution
     */
    Resolution resolution;

    /**
     *  Width in pixels of the encoded image
     */
    size_t width;

    /**
     *  Height in pixels of the encoded image
     */
    size_t height;

    /**
     *  Number of bytes used to encode each pixel in the image
     */
    size_t bytesPerPixel;

    /**
     *  Number of frames contained in the image
     */
    unsigned int frameCount;

    ImageMetadata() : width(0), height(0), bytesPerPixel(0), frameCount(1) {}
    ImageMetadata(Resolution resolution,
                  size_t width,
                  size_t height,
                  size_t bytesPerPixel,
                  unsigned int frameCount)
      : resolution(resolution),
        width(width),
        height(height),
        bytesPerPixel(bytesPerPixel),
        frameCount(frameCount) {}
  };

} // namespace jxrlib
