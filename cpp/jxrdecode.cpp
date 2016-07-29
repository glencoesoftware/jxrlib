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

#include <iostream>
#include <string>

#include "windowsmediaphoto.h"

#include "CodecFactory.hpp"
#include "Factory.hpp"
#include "FormatConverter.hpp"
#include "FormatError.hpp"
#include "ImageDecoder.hpp"
#include "ImageEncoder.hpp"
#include "Stream.hpp"


using namespace jxrlib;

int main(int argc, char* argv[]) {
  try {
    Factory factory;
    CodecFactory codecFactory;

    std::string inputFile = argv[1];
    ImageDecoder decoder = codecFactory.decoderFromFile(inputFile);
    std::cout << "Opened decoder for file: " << inputFile << std::endl;

    uint32_t frameCount = decoder.getFrameCount();
    std::cout << "Found " << frameCount << " frames" << std::endl;

    std::string outputFile = argv[2];
    std::string extension = outputFile.substr(outputFile.find_last_of(".") + 1);

    for (int i = 0 ; i < frameCount ; ++i) {
        decoder.selectFrame(i);
        FormatConverter converter = codecFactory.createFormatConverter(decoder, extension);
        std::cout << "Created format converter for extension: " << extension << std::endl;
        Stream outputStream = factory.createStreamFromFilename(outputFile);
        std::cout << "Created output stream for file: " << outputFile << std::endl;
        ImageEncoder encoder(outputStream, "." + extension);
        std::cout << "Created image encoder" << std::endl;
        encoder.initializeWithDecoder(decoder);
        encoder.writeSource(converter);
    }

    return 0;
  }
  catch (const FormatError &e) {
    std::cout << "*** ERROR: Unsupported format in JPEG XR ***" << std::endl
              << e.what() << std::endl;
    return WMP_errUnsupportedFormat;
  }
}
