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

%typemap(javabase) jxrlib::FormatError "java.lang.Exception";
%rename(getMessage) jxrlib::FormatError::what();

%typemap(javacode) jxrlib::CodecFactory %{
  public ImageDecoder decoderFromBytes(byte bytes[]) {
    return decoderFromBytes(new String(bytes), bytes.length);
  }

  public ImageDecoder decoderFromFile(java.io.File inputFile) {
    return decoderFromFile(inputFile.getAbsolutePath());
  }
%}

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

  class CodecFactory {
  public:
    jxrlib::ImageDecoder decoderFromFile(std::string inputFile);
    jxrlib::ImageDecoder decoderFromBytes(char bytes[], size_t len);
    jxrlib::FormatConverter createFormatConverter(jxrlib::ImageDecoder& imageDecoder, std::string extension);
  };

  class Factory {
  public:
    jxrlib::Stream createStreamFromFilename(std::string filename);
  };

  class FormatConverter {};

  class FormatError {
  public:
    FormatError(std::string what_arg) : what_arg(what_arg) {}
    std::string what();
  };

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
