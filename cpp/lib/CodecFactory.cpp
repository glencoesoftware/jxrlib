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

#include <stdint.h>

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <memory>
#include <sstream>
#include <string>

#include "JXRGlue.h"
#include "windowsmediaphoto.h"
#include "FormatError.hpp"
#include "Stream.hpp"

namespace jxrlib {

  CodecFactory::CodecFactory() : err(WMP_errSuccess) {
    pCodecFactory = NULL;
    Call(PKCreateCodecFactory(&pCodecFactory, WMP_SDK_VERSION));
    return;
  Cleanup:
    throw FormatError("ERROR: Unable to instantiate CodecFactory.");
  }

  CodecFactory::~CodecFactory() {
    if (pCodecFactory) {
      pCodecFactory->Release(&pCodecFactory);
    }
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

  ImageDecoder CodecFactory::decoderFromFile(std::string inputFile, long offset) {
    ImageDecoder decoder;

    int err = 0;
    std::string ext = ".jxr";
    const PKIID *pIID = NULL;

    struct WMPStream* pStream = NULL;

    // fake the "jxr" file extension to create the decoder
    Call(GetImageDecodeIID(ext.c_str(), &pIID));
    Call(PKCodecFactory_CreateCodec(pIID, (void**)&decoder.pDecoder));
#ifdef DEBUG
    printf("Made decoder...");
#endif

    // manually create the stream from the file and advance to offset
    Call(CreateWS_File(&pStream, inputFile.c_str(), "rb"));
    err = std::fseek(pStream->state.file.pFile, offset, SEEK_SET);
    if (err != 0) {
      throw FormatError("ERROR: Unable to advance file to desired position.");
    }
#ifdef DEBUG
    printf("Advanced file...");
    printf("First couple characters: %c%c%c", std::fgetc(pStream->state.file.pFile),  std::fgetc(pStream->state.file.pFile), std::fgetc(pStream->state.file.pFile));
#endif

    // initialize the decoder with the prepared stream
    Call(decoder.pDecoder->Initialize(decoder.pDecoder, pStream));
#ifdef DEBUG
    printf("Initialized with the stream...");
    std::fseek(pStream->state.file.pFile, -3, SEEK_CUR);
#endif
    decoder.pDecoder->fStreamOwner = !0;
    decoder.initialize();

    return decoder;
  Cleanup:
    std::stringstream msg;
    msg << "ERROR: Unable to create decoder from file: " << inputFile << " at offset: " << offset;
    throw FormatError(msg.str().c_str());
  }

  ImageDecoder CodecFactory::decoderFromBytes(std::vector<unsigned char> data) {
    return decoderFromBytes(data.data(), data.size());
  }

  ImageDecoder CodecFactory::decoderFromBytes(unsigned char *bytes, size_t len) {
    ImageDecoder decoder;
    Stream dataStream(bytes, len);
    const PKIID *pIID = NULL;

    Call(GetImageDecodeIID((const char *)".jxr", &pIID));
    Call(PKCodecFactory_CreateCodec(pIID, (void**)&decoder.pDecoder));
    decoder.initialize(dataStream);
    return decoder;
  Cleanup:
    throw FormatError("ERROR: Unable to create decoder from bytes in memory");
  }

  FormatConverter CodecFactory::createFormatConverter(ImageDecoder &imageDecoder,
                                        std::string extension) {
    FormatConverter converter;

    Call(pCodecFactory->CreateFormatConverter(&converter.pConverter));
    Call(converter.pConverter->Initialize(converter.pConverter,
                                          imageDecoder.pDecoder,
                                          (char *) extension.c_str(),
                                          imageDecoder.pDecoder->guidPixFormat));
    return converter;
  Cleanup:
    std::string msg = "ERROR: Unable to create format converter for extension: " + extension;
    throw FormatError(msg);
  }

} // namespace jxrlib
