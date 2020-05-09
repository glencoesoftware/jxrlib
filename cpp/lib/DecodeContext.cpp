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

#include <sstream>
#include "CodecFactory.hpp"
#include "DecodeContext.hpp"
#include "ImageDecoder.hpp"
#include "FormatError.hpp"
#include "JXRGlue.h"

namespace jxrlib {

  signed char* DecodeContext::decodeFirstFrame(char *source,
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
    decoder.selectFrame(0);
    decoder.getRawBytes((unsigned char *)destination);
    return destination;
  }

  std::vector<unsigned char> DecodeContext::decodeFirstFrame(std::vector<unsigned char> &image) {
    ERR err = WMP_errSuccess;
    ImageDecoder decoder;
    CodecFactory codecFactory;
    codecFactory.decoderFromVector(decoder, image);
    decoder.selectFrame(0);
    return decoder.getRawBytes();
  Cleanup:
    std::stringstream msg;
    msg << "ERROR: Could not decode image: " << err;
    std::string errMsg = msg.str();
    throw FormatError(errMsg);
  }

} // namespace jxrlib
