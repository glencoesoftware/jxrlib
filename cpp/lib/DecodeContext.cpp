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
#include "Factory.hpp"
#include "ImageDecoder.hpp"
#include "ImageEncoder.hpp"

namespace jxrlib {

  void DecodeContext::decodeFrame(int frame,
                                  ImageDecoder *source,
                                  signed char *destination) {
    source->selectFrame(frame);
    source->getRawBytes((unsigned char *)destination);
  }

  signed char* DecodeContext::decodeFrame(int frame,
                                          std::string inputFile,
                                          size_t *size) {
    ImageDecoder decoder;
    CodecFactory codecFactory;
    codecFactory.decoderFromFile(decoder, inputFile);
    ImageMetadata metadata = decoder.getImageMetadata();

    *size = metadata.height * metadata.width * metadata.bytesPerPixel;
    signed char *destination = new signed char[*size];
    decodeFrame(frame, &decoder, destination);
    return destination;
  }

  void DecodeContext::decodeFrame(int frame,
                                  std::string inputFile,
                                  std::string outputFile) {
    Factory factory;
    CodecFactory codecFactory;

    ImageDecoder decoder;
    codecFactory.decoderFromFile(decoder, inputFile);
    decoder.selectFrame(frame);

    std::string extension = outputFile.substr(outputFile.find_last_of(".") + 1);
    FormatConverter converter = codecFactory.createFormatConverter(decoder, extension);
    Stream outputStream = factory.createStreamFromFilename(outputFile);
    ImageEncoder encoder(outputStream, "." + extension);

    encoder.initializeWithDecoder(decoder);
    encoder.writeSource(converter);
    encoder.close();
  }

  void DecodeContext::decodeFrame(int frame,
                                  std::string inputFile,
                                  size_t offset,
                                  unsigned char *destination) {
    Factory factory;
    CodecFactory codecFactory;

    ImageDecoder decoder;
    codecFactory.decoderFromFile(decoder, inputFile, offset);
    decodeFrame(frame, &decoder, (signed char *)destination);
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

  ImageMetadata DecodeContext::getImageMetadata(char *source,
                                                size_t offset,
                                                size_t length) {
    ImageDecoder decoder;
    CodecFactory codecFactory;
    codecFactory.decoderFromBytes(
      decoder, (unsigned char *)source, offset, length);
    return decoder.getImageMetadata();
  }

  ImageMetadata DecodeContext::getImageMetadata(std::string inputFile,
                                                size_t offset) {
    ImageDecoder decoder;
    CodecFactory codecFactory;
    codecFactory.decoderFromFile(decoder, inputFile, offset);
    return decoder.getImageMetadata();
  }
} // namespace jxrlib
