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
#include <cstring>
#include <memory>

#include "JXRGlue.h"
#include "windowsmediaphoto.h"
#include "FormatError.hpp"
#include "Stream.hpp"

namespace jxrlib {

  /* XXX: Debugging to be removed. */
  const int MOD_ADLER = 65521;

  uint32_t adler32(unsigned char *data, size_t len)
  {
      uint32_t a = 1, b = 0;
      size_t index;

      /* Process each byte of the data in order */
      for (index = 0; index < len; ++index)
      {
          a = (a + data[index]) % MOD_ADLER;
          b = (b + a) % MOD_ADLER;
      }

      return (b << 16) | a;
  }
  /* XXX: Debugging to be removed. */

  using namespace std;

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

  ImageDecoder CodecFactory::decoderFromBytes(std::vector<unsigned char> data) {
    return decoderFromBytes((char *)data.data(), data.size());
  }

  ImageDecoder CodecFactory::decoderFromBytes(char *bytes, size_t len) {
    ImageDecoder decoder;
    cerr << "CodecFactory::decoderFromBytes checksum: "
         << adler32((unsigned char *)bytes, len) << endl;
    Stream dataStream((unsigned char *)bytes, len);
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
