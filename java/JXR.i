%module JXR
%include typemaps.i
%include std_vector.i
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
  %typemap(javacode) CodecFactory %{
  public ImageDecoder decoderFromBytes(byte bytes[]) {
    return decoderFromBytes(new String(bytes), bytes.length);
  }

  public ImageDecoder decoderFromFile(java.io.File inputFile) {
    return decoderFromFile(inputFile.getAbsolutePath());
  }
  %}
  class CodecFactory {
  public:
    jxrlib::ImageDecoder decoderFromFile(std::string inputFile);
    jxrlib::ImageDecoder decoderFromBytes(char bytes[], size_t len);
    jxrlib::FormatConverter createFormatConverter(jxrlib::ImageDecoder& imageDecoder, std::string extension);
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
    unsigned int getWidth();
    unsigned int getHeight();
    jxrlib::Resolution getResolution();
    std::vector<char> getRawBytes();
    void close();
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
