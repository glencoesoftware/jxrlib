%module JXRLib
%include std_string.i
%{
  #include "CodecFactory.hpp"
  #include "Factory.hpp"
  #include "FormatConverter.hpp"
  #include "FormatError.hpp"
  #include "ImageDecoder.hpp"
  #include "ImageEncoder.hpp"
  #include "Resolution.hpp"
  #include "Stream.hpp"
%}
class CodecFactory {
 public:
  ImageDecoder decoderFromFile(std::string inputFile);
  FormatConverter createFormatConverter(ImageDecoder& imageDecoder, std::string extension);
};

class Factory {
 public:
  Stream createStreamFromFilename(std::string filename);
};

class FormatConverter {};

class FormatError {};

class ImageDecoder {
 public:
  void initialize();
  uint32_t getFrameCount();
  void selectFrame(uint32_t frameNum);
  GUID getGUIDPixFormat();
  bool getBlackWhite();
  uint32_t getWidth();
  uint32_t getHeight();
  Resolution getResolution();
};

class ImageEncoder {
 public:
  void initializeWithDecoder(ImageDecoder& decoder);
  void writeSource(FormatConverter& converter);
};

struct Resolution {};

struct Stream {};
