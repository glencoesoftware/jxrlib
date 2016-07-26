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

#include <cstring>
#include <memory>

#include "JXRGlue.h"
#include "windowsmediaphoto.h"
#include "FormatError.hpp"

namespace jxrlib {

  CodecFactory::CodecFactory() : err(WMP_errSuccess) {
    pCodecFactory = NULL;
    Call(PKCreateCodecFactory(&pCodecFactory, WMP_SDK_VERSION));
    return;
  Cleanup:
    throw FormatError("ERROR: Unable to instantiate CodecFactory.");
  }

  ImageDecoder CodecFactory::decoderFromFile(std::string inputFile) {
    ImageDecoder decoder;
    Call(pCodecFactory->CreateDecoderFromFile(inputFile.c_str(), &decoder.pDecoder));
    decoder.initialize();
    return decoder;
  Cleanup:
    std::string msg = "ERROR: Unable to create decoder from file: " + inputFile;
    throw FormatError(msg);
  }

  FormatConverter CodecFactory::createFormatConverter(ImageDecoder imageDecoder,
                                        std::string extension) {
    FormatConverter converter;
    int extLength = extension.length();
    char *ext = new char[extLength + 1];
    strncpy(ext, extension.c_str(), extLength);

    Call(pCodecFactory->CreateFormatConverter(&converter.pConverter));
    Call(converter.pConverter->Initialize(converter.pConverter,
                                          imageDecoder.pDecoder,
                                          ext,
                                          imageDecoder.pDecoder->guidPixFormat));
    delete[] ext;
    return converter;
  Cleanup:
    delete[] ext;
    std::string msg = "ERROR: Unable to create format converter for extension: " + extension;
    throw FormatError(msg);
  }

} // namespace jxrlib
