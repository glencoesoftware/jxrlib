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

  /**
   *  The DecodeContext class implements a number of convenience functions that
   *  allow for single-call decoding of JPEG XR images in a variety of different
   *  contexts. Note that methods are not re-entrant or thread-safe, and assume
   *  that arguments passed in will not move/change during decode, as no copying
   *  or locking is performed.
   */
  class DecodeContext {
  public:

    /**
     *  Decodes a single frame from the JPEG XR image used to initialize an
     *  ImageDecoder
     *
     *  @param frame       The (0-based) index of the frame to decode
     *  @param source      ImageDecoder initialized with the image to decode
     *  @param destination Pointer to pre-allocated memory where decoded image
     *                     should be written
     */
    void decodeFrame(int frame,
                     ImageDecoder &source,
                     signed char *destination);

    /**
     *  Decodes a single frame from a JPEG XR file
     *
     *  @param      frame     The (0-based) index of the frame to decode
     *  @param      inputFile Path to the JPEG XR file to decode
     *  @param[out] size      The size of the decoded image
     *  @return Pointer to the memory containing the decoded image data
     */
    signed char *decodeFrame(int frame,
                             std::string inputFile,
                             size_t *size);

    /**
     *  Transcodes a single frame from a JPEG XR file to the format specified by
     *  the output file's extension
     *
     *  @param frame      The (0-based) index of the frame to decode
     *  @param inputFile  Path to the JPEG XR file to decode
     *  @param outputFile Path where the transcoded image will be written
     */
    void decodeFrame(int frame,
                     std::string inputFile,
                     std::string outputFile);

    /**
     *  Decodes a single frame from a file containing JPEG XR data
     *
     *  @param frame       The (0-based) index of the frame to decode
     *  @param inputFile   Path to the file containing the JPEG XR data
     *  @param offset      Location within the file where the JPEG XR data begins
     *  @param destination Pointer to pre-allocated memory where decoded image
     *                     should be written
     */
    void decodeFrame(int frame,
                     std::string inputFile,
                     size_t offset,
                     unsigned char *destination);

    /**
     *  Decodes a single frame from data containing a JPEG XR image
     *
     *  @param frame             The (0-based) index of the frame to decode
     *  @param source            Location of the data containing the JPEG XR
     *                           image
     *  @param sourceOffset      Offset within the source data where image data
     *                           begins
     *  @param sourceLength      Length of the JPEG XR image data
     *  @param destination       Pointer to pre-allocated memory where decoded
     *                           image should be written
     *  @param destinationOffset Offset from the destination where image data
     *                           should start
     */
    void decodeFrame(int frame,
                     unsigned char *source,
                     size_t sourceOffset,
                     size_t sourceLength,
                     unsigned char *destination,
                     size_t destinationOffset);

    /**
     *  Decodes a single frame from JPEG XR image data
     *
     *  @param      frame  The (0-based) index of the frame to decode
     *  @param      source Location of the data containing the JPEG XR image
     *  @param      offset Offset within the source data where image data begins
     *  @param      length Length of the JPEG XR image data
     *  @param[out] size   The size of the decoded image
     *  @return Pointer to the memory containing the decoded image data
     */
    signed char* decodeFrame(int frame,
                             char *source,
                             size_t offset,
                             size_t length,
                             size_t *size);

    /**
     *  Extracts metadata from JPEG XR image data
     *
     *  @param source Location of the data containing the JPEG XR image
     *  @param offset Offset within the source data where image data begins
     *  @param length Length of the JPEG XR image data
     *  @return An ImageMetadata object instantiated with the extracted image
     *          parameters
     */
    ImageMetadata getImageMetadata(char *source,
                                   size_t offset,
                                   size_t length);
    /**
     *  Extracts metadata from a JPEG XR file
     *
     *  @param inputFile  Path to the JPEG XR file to decode
     *  @param offset     Location within the file where the JPEG XR data begins
     *  @return An ImageMetadata object instantiated with the extracted image
     *          parameters
     */
    ImageMetadata getImageMetadata(std::string inputFile,
                                   size_t offset);
  };

} // namespace jxrlib
