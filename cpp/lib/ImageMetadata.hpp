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

  struct ImageMetadata {
    Resolution resolution;
    size_t width;
    size_t height;
    size_t bytesPerPixel;
    ImageMetadata() : width(0), height(0), bytesPerPixel(0) {}
    ImageMetadata(Resolution resolution,
                  size_t width,
                  size_t height,
                  size_t bytesPerPixel)
      : resolution(resolution),
        width(width),
        height(height),
        bytesPerPixel(bytesPerPixel) {}
  };

} // namespace jxrlib
