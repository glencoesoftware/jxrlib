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
#include "Resolution.hpp"
#include "Stream.hpp"

namespace jxrlib {

  class ImageDecoder {
    PKImageDecode *pDecoder;
    ERR err;
    PKPixelInfo pixelInfo;
    friend class CodecFactory;
  public:
    ImageDecoder() : pDecoder(NULL), err(WMP_errSuccess) {};
    ~ImageDecoder();
    void initialize();
    void initialize(Stream &data);

    unsigned int getFrameCount();
    void selectFrame(unsigned int frameNum);

    GUID getGUIDPixFormat();
    bool getBlackWhite();
    size_t getWidth();
    size_t getHeight();
    size_t getBytesPerPixel();
    Resolution getResolution();
    void getRawBytes(unsigned char *image_buffer);
    std::vector<unsigned char> getRawBytes();
  };

} // namespace jxrlib
