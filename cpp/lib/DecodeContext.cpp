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

#include "CodecFactory.hpp"
#include "DecodeContext.hpp"
#include "ImageDecoder.hpp"

namespace jxrlib {

  void DecodeContext::decodeFrame(int frame,
                                  ImageDecoder *source,
                                  signed char *destination) {
    source->selectFrame(frame);
    source->getRawBytes((unsigned char *)destination);
  }

  void DecodeContext::decodeFrame(int frame,
                                  unsigned char *source,
                                  size_t sourceOffset,
                                  size_t sourceLength,
                                  unsigned char *destination,
                                  size_t destinationOffset) {
    ImageDecoder decoder;
    CodecFactory codecFactory;
    codecFactory.decoderFromBytes(
      decoder, source, sourceOffset, sourceLength);

    this->decodeFrame(
      frame, &decoder, (signed char *)(destination + destinationOffset));
  }

  signed char* DecodeContext::decodeFrame(int frame,
                                          char *source,
                                          size_t offset,
                                          size_t length,
                                          size_t *size) {
    ImageDecoder decoder;
    CodecFactory codecFactory;
    codecFactory.decoderFromBytes(
      decoder, (unsigned char *)source, offset, length);

    *size =
      decoder.getWidth() * decoder.getHeight() * decoder.getBytesPerPixel();
    signed char *destination = new signed char[*size];
    this->decodeFrame(frame, &decoder, destination);
    return destination;
  }

} // namespace jxrlib
