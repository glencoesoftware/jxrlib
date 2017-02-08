%module JXR
%include typemaps.i
%include std_vector.i
%include std_string.i
%include arrays_java.i
%{
  #include "CodecFactory.hpp"
  #include "DecodeContext.hpp"
  #include "Factory.hpp"
  #include "FormatConverter.hpp"
  #include "FormatError.hpp"
  #include "ImageDecoder.hpp"
  #include "ImageEncoder.hpp"
  #include "ImageMetadata.hpp"
  #include "Resolution.hpp"
  #include "Stream.hpp"
%}

/* 
 * char *BYTE typemaps. 
 * These are input typemaps for mapping a Java byte[] array to a C char array.
 * Note that as a Java array is used and thus passeed by reference, the C routine 
 * can return data to Java via the parameter.
 *
 * Example usage wrapping:
 *   void foo(char *array);
 *  
 * Java usage:
 *   byte b[] = new byte[20];
 *   modulename.foo(b);
 */
%typemap(jni) unsigned char *BYTE "jbyteArray"
%typemap(jtype) unsigned char *BYTE "byte[]"
%typemap(jstype) unsigned char *BYTE "byte[]"
%typemap(in) unsigned char *BYTE {
  $1 = (unsigned char *) JCALL2(GetByteArrayElements, jenv, $input, 0); 
}

%typemap(argout) unsigned char *BYTE {
  JCALL3(ReleaseByteArrayElements, jenv, $input, (jbyte *) $1, 0); 
}

%typemap(javain) unsigned char *BYTE "$javainput"

/* Prevent default freearg typemap from being used */
%typemap(freearg) unsigned char *BYTE ""

/* 
 * unsigned char *NIOBUFFER typemaps. 
 * This is for mapping Java nio buffers to C char arrays.
 * It is useful for performance critical code as it reduces the memory copy an marshaling overhead.
 * Note: The Java buffer has to be allocated with allocateDirect.
 *
 * Example usage wrapping:
 *   %apply unsigned char *NIOBUFFER { unsigned char *buf };
 *   void foo(unsigned char *buf);
 *  
 * Java usage:
 *   java.nio.ByteBuffer b = ByteBuffer.allocateDirect(20); 
 *   modulename.foo(b);
 */
%typemap(jni) unsigned char *NIOBUFFER "jobject"  
%typemap(jtype) unsigned char *NIOBUFFER "java.nio.ByteBuffer"  
%typemap(jstype) unsigned char *NIOBUFFER "java.nio.ByteBuffer"  
%typemap(javain,
  pre="  assert $javainput.isDirect() : \"Buffer must be allocated direct.\";") unsigned char *NIOBUFFER "$javainput"
%typemap(javaout) unsigned char *NIOBUFFER {  
  return $jnicall;  
}  
%typemap(in) unsigned char *NIOBUFFER {  
  $1 = (unsigned char *) JCALL1(GetDirectBufferAddress, jenv, $input); 
  if ($1 == NULL) {  
    SWIG_JavaThrowException(jenv, SWIG_JavaRuntimeException, "Unable to get address of a java.nio.ByteBuffer direct byte buffer. Buffer must be a direct buffer and not a non-direct buffer.");  
  }  
}  
%typemap(memberin) unsigned char *NIOBUFFER {  
  if ($input) {  
    $1 = $input;  
  } else {  
    $1 = 0;  
  }  
}  
%typemap(freearg) unsigned char *NIOBUFFER ""  
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
    void decoderFromFile(jxrlib::ImageDecoder& decoder,
                         std::string inputFile) throw(FormatError);
    void decoderFromBytes(jxrlib::ImageDecoder& decoder,
                          unsigned char *NIOBUFFER,
                          size_t offset,
                          size_t length) throw(FormatError);
    jxrlib::FormatConverter createFormatConverter(
      jxrlib::ImageDecoder& decoder,
      std::string extension) throw(FormatError);
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
  struct Resolution {
    float X;
    float Y;
  };

  %typemap(javaclassmodifiers) ImageMetadata "class"
  struct ImageMetadata {
    Resolution resolution;
    size_t width;
    size_t height;
    size_t bytesPerPixel;
  };

  %typemap(javaclassmodifiers) Stream "class"
  struct Stream {};

  %typemap(in,numinputs=0,noblock=1) size_t *size {
    size_t temp_size;
    $1 = &temp_size;
  }
  %typemap(jstype) signed char* decodeFrame "byte[]"
  %typemap(jtype) signed char* decodeFrame "byte[]"
  %typemap(jni) signed char* decodeFrame "jbyteArray"
  %typemap(javaout) signed char* decodeFrame {
    return $jnicall;
  }
  %typemap(out) signed char* decodeFrame {
    $result = JCALL1(NewByteArray, jenv, temp_size);
    if (!$result) return 0;
    JCALL4(SetByteArrayRegion, jenv, $result, 0, temp_size, $1);
    delete[] $1;
  }
  %typemap(javacode) DecodeContext %{
    public ImageMetadata getImageMetadataFromBytes(byte[] source) {
      return new DecodeContext().getImageMetadataFromBytes(source, 0, source.length);
    }
  %}

  class DecodeContext {
  public:
    %rename(transcodeFile) decodeFrame(int frame,
                                       std::string inputFile,
                                       std::string outputFile,
                                       size_t sourceOffset = 0) throw(FormatError);
    void decodeFrame(int frame,
                     std::string inputFile,
                     std::string outputFile,
                     size_t offset = 0) throw(FormatError);

    %rename(decodeFile) decodeFrame(int frame,
                                    std::string inputFile,
                                    size_t *size,
                                    size_t sourceOffset = 0) throw(FormatError);
    signed char* decodeFrame(int frame,
                             std::string inputFile,
                             size_t *size,
                             size_t sourceOffset = 0) throw(FormatError);

    %rename(decodeFileToBytes) decodeFrame(int frame,
                                           std::string inputFile,
                                           unsigned char *BYTE,
                                           size_t sourceOffset = 0) throw(FormatError);
    void decodeFrame(int frame,
                     std::string inputFile,
                     unsigned char *BYTE,
                     size_t sourceOffset = 0) throw(FormatError);

    %rename(decodeFileToBuffer) decodeFrame(int frame,
                                            std::string inputFile,
                                            unsigned char *NIOBUFFER,
                                            size_t sourceOffset = 0) throw(FormatError);
    void decodeFrame(int frame,
                     std::string inputFile,
                     unsigned char *NIOBUFFER,
                     size_t sourceOffset = 0) throw(FormatError);

    %rename(decodeBytes) decodeFrame(int frame,
                                     unsigned char *BYTE,
                                     size_t *size,
                                     size_t sourceOffset,
                                     size_t sourceLength) throw(FormatError);
    signed char* decodeFrame(int frame,
                             unsigned char *BYTE,
                             size_t *size,
                             size_t sourceOffset,
                             size_t sourceLength) throw(FormatError);

    %rename(decodeBuffer) decodeFrame(int frame,
                                      unsigned char *NIOBUFFER,
                                      size_t *size,
                                      size_t sourceOffset,
                                      size_t sourceLength) throw(FormatError);
    signed char* decodeFrame(int frame,
                             unsigned char *NIOBUFFER,
                             size_t *size,
                             size_t sourceOffset,
                             size_t sourceLength) throw(FormatError);

    %rename(decodeBytesToBytes) decodeFrame(int frame,
                                            unsigned char *BYTE,
                                            unsigned char *BYTE,
                                            size_t sourceOffset,
                                            size_t sourceLength,
                                            size_t destinationOffset = 0) throw(FormatError);
    void decodeFrame(int frame,
                     unsigned char *BYTE,
                     unsigned char *BYTE,
                     size_t sourceOffset,
                     size_t sourceLength,
                     size_t destinationOffset = 0) throw(FormatError);

    %rename(decodeBytesToBuffer) decodeFrame(int frame,
                                             unsigned char *BYTE,
                                             unsigned char *NIOBUFFER,
                                             size_t sourceOffset,
                                             size_t sourceLength,
                                             size_t destinationOffset = 0) throw(FormatError);
    void decodeFrame(int frame,
                     unsigned char *BYTE,
                     unsigned char *NIOBUFFER,
                     size_t sourceOffset,
                     size_t sourceLength,
                     size_t destinationOffset = 0) throw(FormatError);

    %rename(decodeBufferToBytes) decodeFrame(int frame,
                                             unsigned char *NIOBUFFER,
                                             unsigned char *BYTE,
                                             size_t sourceOffset,
                                             size_t sourceLength,
                                             size_t destinationOffset = 0) throw(FormatError);
    void decodeFrame(int frame,
                     unsigned char *NIOBUFFER,
                     unsigned char *BYTE,
                     size_t sourceOffset,
                     size_t sourceLength,
                     size_t destinationOffset = 0) throw(FormatError);

    %rename(decodeBufferToBuffer) decodeFrame(int frame,
                                              unsigned char *NIOBUFFER,
                                              unsigned char *NIOBUFFER,
                                              size_t sourceOffset,
                                              size_t sourceLength,
                                              size_t destinationOffset = 0) throw(FormatError);
    void decodeFrame(int frame,
                     unsigned char *NIOBUFFER,
                     unsigned char *NIOBUFFER,
                     size_t sourceOffset,
                     size_t sourceLength,
                     size_t destinationOffset = 0) throw(FormatError);

    %rename(getImageMetadataFromFile) getImageMetadata(std::string inputFile,
                                                       size_t offset = 0) throw(FormatError);
    ImageMetadata getImageMetadata(std::string inputFile,
                                   size_t offset = 0) throw(FormatError);

    %rename(getImageMetadataFromBytes) getImageMetadata(unsigned char *BYTE,
                                                        size_t sourceOffset,
                                                        size_t sourceLength) throw(FormatError);
    ImageMetadata getImageMetadata(unsigned char *BYTE,
                                   size_t sourceOffset,
                                   size_t sourceLength) throw(FormatError);

    %rename(getImageMetadataFromBuffer) getImageMetadata(unsigned char *NIOBUFFER,
                                                         size_t sourceOffset,
                                                         size_t sourceLength) throw(FormatError);
    ImageMetadata getImageMetadata(unsigned char *NIOBUFFER,
                                   size_t sourceOffset,
                                   size_t sourceLength) throw(FormatError);
  };
}
