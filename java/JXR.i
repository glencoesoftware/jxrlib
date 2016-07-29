%module JXR
%include typemaps.i
%include std_string.i
%include arrays_java.i
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

%typemap(javabase) FormatError "java.lang.Exception";
%typemap(javacode) FormatError %{
  public String getMessage() {
    return what();
  }
%}

typedef unsigned int uint32_t;

typedef struct {
  unsigned long  Data1;
  unsigned short Data2;
  unsigned short Data3;
  unsigned char  Data4[ 8 ];
} GUID;

namespace jxrlib {

  class CodecFactory {
  public:
    jxrlib::ImageDecoder decoderFromFile(std::string inputFile);
    jxrlib::FormatConverter createFormatConverter(jxrlib::ImageDecoder& imageDecoder, std::string extension);
  };

  class Factory {
  public:
    jxrlib::Stream createStreamFromFilename(std::string filename);
  };

  class FormatConverter {};

  class FormatError {
  public:
    FormatError(std::string &what_arg) : what_arg(what_arg) {}
    std::string what();
  };

  class ImageDecoder {
  public:
    void initialize();
    uint32_t getFrameCount();
    void selectFrame(uint32_t frameNum);
    GUID getGUIDPixFormat();
    bool getBlackWhite();
    uint32_t getWidth();
    uint32_t getHeight();
    jxrlib::Resolution getResolution();
    void close();
  };

  class ImageEncoder {
  public:
    ImageEncoder(jxrlib::Stream encodeStream, std::string extension);
    void initializeWithDecoder(jxrlib::ImageDecoder& decoder);
    void writeSource(jxrlib::FormatConverter& converter);
    void close();
  };

  struct Resolution {};

  struct Stream {};

}
