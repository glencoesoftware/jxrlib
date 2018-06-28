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

namespace jxrlib {

  /**
   *  The Resolution struct contains information about the resolution of a
   *  JPEG-XR encoded image
   */
  struct Resolution {

    /**
     *  The resolution of the encoded image in the X direction
     */
    float X;

    /**
     *  The resolution of the encoded image in the Y direction
     */
    float Y;
    
    Resolution() : X(0.0), Y(0.0) {}
    Resolution(float X, float Y) : X(X), Y(Y) {}
  };

} // namespace jxrlib
