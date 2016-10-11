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

  %typemap(javabase) FormatError "java.lang.Exception"
  %rename(getMessage) FormatError::what();
  class FormatError {
  public:
    FormatError(std::string what_arg) : what_arg(what_arg) {}
    std::string what();
  };

  %typemap(throws) FormatError {
    jclass exception = jenv->FindClass("ome/jxrlib/FormatError");
    if (exception) {
      jenv->ThrowNew(exception, $1.what());
    }
    return $null;
  }

  %typemap(javaclassmodifiers) CodecFactory "class"
  class CodecFactory {
  public:
    void decoderFromFile(jxrlib::ImageDecoder& decoder, std::string inputFile) throw(FormatError);
    void decoderFromBytes(jxrlib::ImageDecoder& decoder, unsigned char *NIOBUFFER, size_t len) throw(FormatError);
    jxrlib::FormatConverter createFormatConverter(jxrlib::ImageDecoder& decoder, std::string extension) throw(FormatError);
  };

  %typemap(javaclassmodifiers) Factory "class"
  class Factory {
  public:
    Factory() throw(FormatError);
    jxrlib::Stream createStreamFromFilename(std::string filename) throw(FormatError);;
  };

  %typemap(javaclassmodifiers) FormatConverter "class"
  class FormatConverter {};

  %typemap(javaclassmodifiers) ImageDecoder "class"
  class ImageDecoder {
  public:
    void initialize() throw(FormatError);
    unsigned int getFrameCount() throw(FormatError);
    void selectFrame(unsigned int frameNum) throw(FormatError);
    GUID getGUIDPixFormat();
    bool getBlackWhite();
    size_t getWidth();
    size_t getHeight();
    size_t getBytesPerPixel();
    jxrlib::Resolution getResolution() throw(FormatError);
    void getRawBytes(unsigned char *NIOBUFFER) throw(FormatError);
  };

  %typemap(javaclassmodifiers) ImageEncoder "class"
  class ImageEncoder {
  public:
    ImageEncoder(jxrlib::Stream encodeStream, std::string extension) throw(FormatError);
    void initializeWithDecoder(jxrlib::ImageDecoder& decoder) throw(FormatError);
    void writeSource(jxrlib::FormatConverter& converter) throw(FormatError);
    void close();
  };

  %typemap(javaclassmodifiers) Resolution "class"
  struct Resolution {};

  %typemap(javaclassmodifiers) Stream "class"
  struct Stream {};

}
