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
#include <sstream>

#include <openssl/md5.h>

#include "windowsmediaphoto.h"

#include "CodecFactory.hpp"
#include "Factory.hpp"
#include "FormatConverter.hpp"
#include "FormatError.hpp"
#include "ImageDecoder.hpp"
#include "ImageEncoder.hpp"
#include "Stream.hpp"


using namespace jxrlib;

template<typename T>
static std::string md5(std::vector<T> bytes, size_t offset) {
  std::vector<unsigned char> md5_bytes(MD5_DIGEST_LENGTH);
  MD5(
    (const unsigned char *)bytes.data() + offset,
    bytes.size() - offset,
    md5_bytes.data());
  std::stringstream ss;
  for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
    ss << std::hex << (int)md5_bytes[i];
  }
  return ss.str();
}

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
  std::vector<unsigned char> decodedBytes(frameSize);

  unsigned int frameCount = decoder.getFrameCount();
  std::cerr << "Found " << frameCount << " frames" << std::endl;

  for(int i = 0 ; i < frameCount ; i++) {
    decoder.selectFrame(i);
    decoder.getRawBytes(decodedBytes.data());

    std::cerr << decodedBytes.size() << " Bytes:" << std::endl;
    print_bytes(decodedBytes, stdout);
  }
}

void stream_file_bytes(std::string inputFile, size_t offset) {
  std::ifstream input(inputFile.c_str(), std::ios::binary|std::ios::ate);
  std::ifstream::pos_type endPos = input.tellg();
  std::vector<char> bytes(offset + endPos);
  size_t length = bytes.size() - offset;

  std::cerr << "Reading bytes from file: " << inputFile << std::endl;
  input.seekg(0, std::ios::beg);
  input.read(bytes.data() + offset, length);

  std::cerr << "Read bytes MD5: " << md5(bytes, offset) << std::endl;

  Factory factory;
  CodecFactory codecFactory;

  ImageDecoder decoder;
  codecFactory.decoderFromBytes(
    decoder, (unsigned char *)bytes.data(), offset, length);
  std::cerr << "Opened decoder with " << length << " bytes" << std::endl;

  unsigned int frameSize =
    decoder.getWidth() * decoder.getHeight() * decoder.getBytesPerPixel();
  std::vector<unsigned char> decodedBytes(frameSize);

  unsigned int frameCount = decoder.getFrameCount();
  std::cerr << "Found " << frameCount << " frames" << std::endl;

  for(int i = 0 ; i < frameCount ; i++) {
    decoder.selectFrame(i);
    decoder.getRawBytes(decodedBytes.data());

    std::cerr << decodedBytes.size() << " Bytes MD5: " << md5(decodedBytes, 0)
      << std::endl;
  }
}

void stream_file(std::string inputFile) {
  Factory factory;
  CodecFactory codecFactory;

  ImageDecoder decoder;
  codecFactory.decoderFromFile(decoder, inputFile);
  std::cerr << "Opened decoder for file: " << inputFile << std::endl;

  unsigned int frameSize =
    decoder.getWidth() * decoder.getHeight() * decoder.getBytesPerPixel();
  std::vector<unsigned char> bytes(frameSize);

  unsigned int frameCount = decoder.getFrameCount();
  std::cerr << "Found " << frameCount << " frames" << std::endl;

  for(int i = 0 ; i < frameCount ; i++) {
    decoder.selectFrame(i);
    decoder.getRawBytes(bytes.data());

    std::cerr << bytes.size() << " Bytes:" << std::endl;
    print_bytes(bytes);
  }
}

void stream_file(std::string inputFile, long offset) {
  Factory factory;
  CodecFactory codecFactory;

  ImageDecoder decoder;
  codecFactory.decoderFromFile(decoder, inputFile, offset);
  std::cerr << "Opened decoder for file: " << inputFile << " at offset: " << offset << std::endl;

  unsigned int frameSize =
    decoder.getWidth() * decoder.getHeight() * decoder.getBytesPerPixel();
  std::vector<unsigned char> bytes(frameSize);

  unsigned int frameCount = decoder.getFrameCount();
  std::cerr << "Found " << frameCount << " frames" << std::endl;

  for(int i = 0 ; i < frameCount ; i++) {
    decoder.selectFrame(i);
    decoder.getRawBytes(bytes.data());

    std::cerr << bytes.size() << " Bytes:" << std::endl;
    print_bytes(bytes);
  }
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
      if (std::string(argv[1]) == "--in-memory") {
        stream_file_bytes(argv[2], 0);
      } else if (std::string(argv[1]) == "--in-memory-offset") {
        stream_file_bytes(argv[2], 1024 * 1024);
      }else {
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
