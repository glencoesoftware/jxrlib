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

#include "ImageMetadata.hpp"
#include "JXRGlue.h"
#include "Resolution.hpp"
#include "Stream.hpp"

namespace jxrlib {

  /**
   *  The ImageDecoder class wraps the `pDecoder` struct and corresponding
   *  methods from the jxrlib C code. It contains methods to extract information
   *  from JPEG XR data as well as to decode an image to raw bytes.
   */
  class ImageDecoder {
    PKImageDecode *pDecoder;
    ERR err;
    PKPixelInfo pixelInfo;
    friend class CodecFactory;
  public:
    ImageDecoder() : pDecoder(NULL), err(WMP_errSuccess) {};
    ~ImageDecoder();

    /**
     *  Initializes the underlying `pDecoder` and related structs for use by
     *  setting a number of sensible defaults on a variety of properties
     */
    void initialize();

    /**
     *  Initializes the underlying `pDecoder` in the same manner as initialize()
     *  but also associates the passed in stream with the ImageDecoder such that
     *  it is ready to decode
     *
     *  @param data A Stream initialized with the image data to decode
     */
    void initialize(Stream &data);

    /**
     *  Getter for the number of frames available for decoding
     *
     *  @return The total number of frames
     */
    unsigned int getFrameCount();


    /**
     *  Selects which frame in the image to be decoded will be active for all
     *  subsequent getter/decode method calls
     *
     *  @param frameNum The 0-based integer index of the frame to decode
     */
    void selectFrame(unsigned int frameNum);

    /**
     *  Getter for the GUID assigned to the pixel format of the image to be decoded
     *
     *  @return GUID of the pixel format
     */
    GUID getGUIDPixFormat();

    /**
     *  Getter for whether or not the image is b/w only
     *
     *  @return Boolean indicating if the image is black and white
     */
    bool getBlackWhite();

    /**
     *  Getter for the width of the image to be decoded
     *
     *  @return The width in pixels
     */
    size_t getWidth();

    /**
     *  Getter for the height of the image to be decoded
     *
     *  @return The height in pixels
     */
    size_t getHeight();

    /**
     *  Getter for the size of each pixel in bytes for the image to be decoded
     *
     *  @return The number of bytes in each pixel
     */
    size_t getBytesPerPixel();

    /**
     *  Getter for the resolution of the image to be decoded
     *
     *  @return A Resolution struct containing X and Y resolutions
     */
    Resolution getResolution();

    /**
     *  Getter for the metadata of the image to be decoded
     *
     *  @return An ImageMetadata struct containing the metadata values
     */
    ImageMetadata getImageMetadata();

    /**
     *  Copies the raw, decoded bytes for the image associated with this Image
     *  Decoder into the provided buffer
     *
     *  NOTE: Calling this method causes the image to actually be decoded and
     *  may incur some non-trivial overhead depending on the size of the image
     *
     *  @param[out] image_buffer A pre-allocated buffer to hold the decoded
     *                           image bytes. The buffer MUST be pre-allocated
     *                           with sufficient capacity to hold the entire
     *                           frame being decoded.
     */
    void getRawBytes(unsigned char *image_buffer);
  };

} // namespace jxrlib
