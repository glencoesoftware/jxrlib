%module JXR
%include typemaps.i
%include std_vector.i
%include std_string.i
%include arrays_java.i
%include various.i
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

%pragma(java) moduleclassmodifiers="class"
%pragma(java) jniclassclassmodifiers="class"

namespace std {
  %template(ImageData) vector<char>;
}

typedef struct {
  unsigned long  Data1;
  unsigned short Data2;
  unsigned short Data3;
  unsigned char  Data4[ 8 ];
} GUID;

namespace jxrlib {

  %typemap(javaclassmodifiers) CodecFactory "class"
  class CodecFactory {
  public:
    void decoderFromFile(jxrlib::ImageDecoder& decoder, std::string inputFile);
    void decoderFromBytes(jxrlib::ImageDecoder& decoder, unsigned char *NIOBUFFER, size_t len);
    jxrlib::FormatConverter createFormatConverter(jxrlib::ImageDecoder& decoder, std::string extension);
  };

  %typemap(javaclassmodifiers) Factory "class"
  class Factory {
  public:
    jxrlib::Stream createStreamFromFilename(std::string filename);
  };

  %typemap(javaclassmodifiers) FormatConverter "class"
  class FormatConverter {};

  %typemap(javaclassmodifiers) FormatError "class"
  %typemap(javabase) FormatError "java.lang.Exception";
  %rename(getMessage) FormatError::what();
  class FormatError {
  public:
    FormatError(std::string what_arg) : what_arg(what_arg) {}
    std::string what();
  };

  %typemap(javaclassmodifiers) ImageDecoder "class"
  class ImageDecoder {
  public:
    void initialize();
    unsigned int getFrameCount();
    void selectFrame(unsigned int frameNum);
    GUID getGUIDPixFormat();
    bool getBlackWhite();
    size_t getWidth();
    size_t getHeight();
    size_t getBytesPerPixel();
    jxrlib::Resolution getResolution();
    void getRawBytes(unsigned char *NIOBUFFER);
  };

  %typemap(javaclassmodifiers) ImageEncoder "class"
  class ImageEncoder {
  public:
    ImageEncoder(jxrlib::Stream encodeStream, std::string extension);
    void initializeWithDecoder(jxrlib::ImageDecoder& decoder);
    void writeSource(jxrlib::FormatConverter& converter);
    void close();
  };

  %typemap(javaclassmodifiers) Resolution "class"
  struct Resolution {};

  %typemap(javaclassmodifiers) Stream "class"
  struct Stream {};

}
