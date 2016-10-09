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

#include <cstdio>
#include <cstring>
#include <fstream>
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

static void print_bytes(std::vector<char>bytes, std::FILE *out = stderr) {
  for (int i = 0 ; i < bytes.size() ; ){
    fprintf(out, "0x%.2x%.2x%.2x%.2x%c", bytes[i++], bytes[i++], bytes[i++], bytes[i++],
            i % 40 == 0 ? '\n' : ' ');
  }
}

static void print_bytes(std::vector<unsigned char>bytes, std::FILE *out = stderr) {
  for (int i = 0 ; i < bytes.size() ; ){
    fprintf(out, "0x%.2x%.2x%.2x%.2x%c", bytes[i++], bytes[i++], bytes[i++], bytes[i++],
            i % 40 == 0 ? '\n' : ' ');
  }
}

void stream_data() {
  Factory factory;
  CodecFactory codecFactory;
  std::vector<unsigned char> bytes;

  std::cin.unsetf(std::ios_base::skipws);
  for(unsigned char val ; std::cin >> val ; ){
    bytes.push_back(val);
  }

  std::cerr << "Read bytes:" << std::endl;
  print_bytes(bytes);
  std::cerr << std::endl;

  ImageDecoder decoder;
  codecFactory.decoderFromBytes(decoder, bytes);
  std::cerr << "Opened decoder with " << bytes.size() << " bytes" << std::endl;

  unsigned int frameSize =
    decoder.getWidth() * decoder.getHeight() * decoder.getBytesPerPixel();
  unsigned char *image_buffer = new unsigned char[frameSize];

  unsigned int frameCount = decoder.getFrameCount();
  std::cerr << "Found " << frameCount << " frames" << std::endl;

  for(int i = 0 ; i < frameCount ; i++) {
    decoder.selectFrame(i);
    decoder.getRawBytes(image_buffer);
    std::vector<unsigned char> decoded_bytes(
      image_buffer, image_buffer + frameSize);

    std::cerr << decoded_bytes.size() << " Bytes:" << std::endl;
    print_bytes(decoded_bytes, stdout);
  }

  delete image_buffer;
}

void stream_file_bytes(std::string inputFile) {
  std::ifstream input(inputFile.c_str(), std::ios::binary|std::ios::ate);
  std::ifstream::pos_type endPos = input.tellg();
  std::vector<char> bytes(endPos);

  std::cerr << "Reading bytes from file: " << inputFile << std::endl;
  input.seekg(0, std::ios::beg);
  input.read(&bytes[0], endPos);

  std::cerr << "Read bytes:" << std::endl;
  print_bytes(bytes);
  std::cerr << std::endl;

  Factory factory;
  CodecFactory codecFactory;

  ImageDecoder decoder;
  codecFactory.decoderFromBytes(decoder, (unsigned char *)bytes.data(), bytes.size());
  std::cerr << "Opened decoder with " << bytes.size() << " bytes" << std::endl;

  unsigned int frameSize =
    decoder.getWidth() * decoder.getHeight() * decoder.getBytesPerPixel();
  unsigned char *image_buffer = new unsigned char[frameSize];

  unsigned int frameCount = decoder.getFrameCount();
  std::cerr << "Found " << frameCount << " frames" << std::endl;

  for(int i = 0 ; i < frameCount ; i++) {
    decoder.selectFrame(i);
    decoder.getRawBytes(image_buffer);
    std::vector<unsigned char> decoded_bytes(
      image_buffer, image_buffer + frameSize);

    std::cerr << decoded_bytes.size() << " Bytes:" << std::endl;
    print_bytes(decoded_bytes, stdout);
  }

  delete image_buffer;
}

void stream_file(std::string inputFile) {
  Factory factory;
  CodecFactory codecFactory;

  ImageDecoder decoder;
  codecFactory.decoderFromFile(decoder, inputFile);
  std::cerr << "Opened decoder for file: " << inputFile << std::endl;

  unsigned int frameSize =
    decoder.getWidth() * decoder.getHeight() * decoder.getBytesPerPixel();
  unsigned char *image_buffer = new unsigned char[frameSize];

  unsigned int frameCount = decoder.getFrameCount();
  std::cerr << "Found " << frameCount << " frames" << std::endl;

  for(int i = 0 ; i < frameCount ; i++) {
    decoder.selectFrame(i);
    decoder.getRawBytes(image_buffer);
    std::vector<unsigned char> bytes(
      image_buffer, image_buffer + frameSize);

    std::cerr << bytes.size() << " Bytes:" << std::endl;
    print_bytes(bytes);
  }

  delete image_buffer;
}

void stream_file(std::string inputFile, long offset) {
  Factory factory;
  CodecFactory codecFactory;

  ImageDecoder decoder;
  codecFactory.decoderFromFile(decoder, inputFile, offset);
  std::cerr << "Opened decoder for file: " << inputFile << " at offset: " << offset << std::endl;

  unsigned int frameSize =
    decoder.getWidth() * decoder.getHeight() * decoder.getBytesPerPixel();
  unsigned char *image_buffer = new unsigned char[frameSize];

  unsigned int frameCount = decoder.getFrameCount();
  std::cerr << "Found " << frameCount << " frames" << std::endl;

  for(int i = 0 ; i < frameCount ; i++) {
    decoder.selectFrame(i);
    decoder.getRawBytes(image_buffer);
    std::vector<unsigned char> bytes(
      image_buffer, image_buffer + frameSize);

    std::cerr << bytes.size() << " Bytes:" << std::endl;
    print_bytes(bytes);
  }

  delete image_buffer;
}

void convert_file(std::string inputFile, std::string outputFile) {
  Factory factory;
  CodecFactory codecFactory;

  ImageDecoder decoder;
  codecFactory.decoderFromFile(decoder, inputFile);
  std::cerr << "Opened decoder for file: " << inputFile << std::endl;

  unsigned int frameCount = decoder.getFrameCount();
  std::cerr << "Found " << frameCount << " frames" << std::endl;

  std::string extension = outputFile.substr(outputFile.find_last_of(".") + 1);

  for(int i = 0 ; i < frameCount ; ++i) {
    decoder.selectFrame(i);
    FormatConverter converter = codecFactory.createFormatConverter(decoder, extension);
    std::cerr << "Created format converter for extension: " << extension << std::endl;
    Stream outputStream = factory.createStreamFromFilename(outputFile);
    std::cerr << "Created output stream for file: " << outputFile << std::endl;
    ImageEncoder encoder(outputStream, "." + extension);
    std::cerr << "Created image encoder" << std::endl;
    encoder.initializeWithDecoder(decoder);
    encoder.writeSource(converter);
    encoder.close();
  }
}

int main(int argc, char* argv[]) {
  try {
    switch(argc) {
    case 1 :
      std::cerr << "Streaming data in/out..." << std::endl;
      stream_data();
      break;
    case 2 :
      std::cerr << "Streaming file " << argv[1] << " data out..." << std::endl;
      stream_file(argv[1]);
      break;
    case 3 :
      if (std::strncmp(argv[1], "--in-memory", 11) == 0) {
        stream_file_bytes(argv[2]);
      } else {
        std::cerr << "Converting file " << argv[1] << " to " << argv[2] << "..." << std::endl;
        convert_file(argv[1], argv[2]);
      }
      break;
    case 4 :
      if (std::strncmp(argv[1], "--from-czi-offset", 17) == 0) {
        stream_file(argv[2], atol(argv[3]));
      }
      break;
    default : std::cerr << "Incorrect usage..." << std::endl;
    }
  }
  catch (const FormatError &e) {
    std::cerr << "*** ERROR: Unsupported format in JPEG XR ***" << std::endl
              << e.what() << std::endl;
    return WMP_errUnsupportedFormat;
  }
  return 0;
}
