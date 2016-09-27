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

#include "ImageDecoder.hpp"

#include <sstream>
#include <string>

#include "windowsmediaphoto.h"
#include "guiddef.h"
#include "FormatError.hpp"
#include "JXRGlue.h"
#include "Stream.hpp"

namespace jxrlib {

  void ImageDecoder::initialize() {
    // set default color format
    pixelInfo.pGUIDPixFmt = &pDecoder->guidPixFormat;
    Call(PixelFormatLookup(&pixelInfo, LOOKUP_FORWARD));
    Call(PixelFormatLookup(&pixelInfo, LOOKUP_BACKWARD_TIF));

    // color transcoding
    if (IsEqualGUID(*pixelInfo.pGUIDPixFmt, GUID_PKPixelFormat8bppGray)
        || IsEqualGUID(*pixelInfo.pGUIDPixFmt, GUID_PKPixelFormat16bppGray)) {
      // ** => Y transcoding
      pDecoder->guidPixFormat = *pixelInfo.pGUIDPixFmt;
      pDecoder->WMP.wmiI.cfColorFormat = Y_ONLY;
    } else if (IsEqualGUID(*pixelInfo.pGUIDPixFmt, GUID_PKPixelFormat24bppRGB)
               && pDecoder->WMP.wmiI.cfColorFormat == CMYK) {
      // CMYK = > RGB
      pDecoder->WMP.wmiI.cfColorFormat = CF_RGB;
      pDecoder->guidPixFormat = *pixelInfo.pGUIDPixFmt;
      pDecoder->WMP.wmiI.bRGB = 1; // RGB
    }

    PixelFormatLookup(&pixelInfo, LOOKUP_FORWARD);

    // alpha
    if (!!(pixelInfo.grBit & PK_pixfmtHasAlpha)) {
      pDecoder->WMP.wmiSCP.uAlphaMode = 2; // default is image & alpha for formats with alpha
    } else {
      pDecoder->WMP.wmiSCP.uAlphaMode = 0; // otherwise, 0
    }

    // copy properties from PixelInfo to Decoder
    pDecoder->WMP.wmiI.cfColorFormat = pixelInfo.cfColorFormat;
    pDecoder->WMP.wmiI.bdBitDepth = pixelInfo.bdBitDepth;
    pDecoder->WMP.wmiI.cBitsPerUnit = pixelInfo.cbitUnit;

    // skip thumbnails
    pDecoder->WMP.wmiI.cThumbnailWidth = pDecoder->WMP.wmiI.cWidth;
    pDecoder->WMP.wmiI.cThumbnailHeight = pDecoder->WMP.wmiI.cHeight;
    pDecoder->WMP.wmiI.bSkipFlexbits = FALSE;

    // no region decoding
    pDecoder->WMP.wmiI.cROILeftX = 0;
    pDecoder->WMP.wmiI.cROITopY = 0;
    pDecoder->WMP.wmiI.cROIWidth = pDecoder->WMP.wmiI.cWidth;
    pDecoder->WMP.wmiI.cROIHeight = pDecoder->WMP.wmiI.cHeight;

    // other default values
    pDecoder->WMP.wmiSCP.bfBitstreamFormat = SPATIAL;
    pDecoder->WMP.wmiSCP.sbSubband = SB_ALL;
    pDecoder->WMP.bIgnoreOverlap = FALSE;
    pDecoder->WMP.wmiI.oOrientation = O_NONE;
    pDecoder->WMP.wmiI.cPostProcStrength = 0;
    pDecoder->WMP.wmiSCP.bVerbose = FALSE;

    // finished initializing
    return;

  Cleanup:
    throw FormatError("ERROR: Unable to initialize decoder.");
  }

  void ImageDecoder::initialize(Stream &data) {
    ERR err = WMP_errSuccess;
    Call(pDecoder->Initialize(pDecoder, data.pStream));
    pDecoder->fStreamOwner = !0;
    initialize();
    return;
  Cleanup:
    std::stringstream msg;
    msg << "ERROR: Unable to initialize decoder with stream: " << err;
    std::string errMsg = msg.str();
    throw FormatError(errMsg);
  }

  unsigned int ImageDecoder::getFrameCount() {
    ERR err = WMP_errSuccess;
    U32 cFrame;
    Call(pDecoder->GetFrameCount(pDecoder, &cFrame));
    return cFrame;
  Cleanup:
    std::stringstream msg;
    msg << "ERROR: Unable to get frame count: " << err;
    std::string errMsg = msg.str();
    throw FormatError(errMsg);
  }

  void ImageDecoder::selectFrame(unsigned int frameNum) {
    Call(pDecoder->SelectFrame(pDecoder, frameNum));
    return;
  Cleanup:
    std::stringstream msg;
    msg << "ERROR: Unable to select frame " << frameNum;
    std::string errMsg = msg.str();
    throw FormatError(errMsg);
  }

  GUID ImageDecoder::getGUIDPixFormat() { return pDecoder->guidPixFormat; }
  bool ImageDecoder::getBlackWhite() { return pDecoder->WMP.wmiSCP.bBlackWhite; }
  unsigned int ImageDecoder::getWidth() { return pDecoder->WMP.wmiI.cROIWidth; }
  unsigned int ImageDecoder::getHeight() { return pDecoder->WMP.wmiI.cROIHeight; }

  Resolution ImageDecoder::getResolution() {
    float rX = 0.0, rY = 0.0;
    Resolution res;
    Call(pDecoder->GetResolution(pDecoder, &rX, &rY));
    res.X = rX;
    res.Y = rY;
    return res;
  Cleanup:
    throw FormatError("ERROR: Could not get decoder resolution");
  }

  void ImageDecoder::close() {
    pDecoder->Release(&pDecoder);
  }

  std::vector<char> ImageDecoder::getRawBytes() {
    int width, height;
    PKRect rc;
    size_t buf_size;
    unsigned char *image_buffer;
    std::vector<char> ret;

    Call(pDecoder->GetSize(pDecoder, &width, &height));
    buf_size = width * height * 4;
    image_buffer = (unsigned char *)malloc(buf_size);
    ret.resize(buf_size);

    rc.X = 0;
    rc.Y = 0;
    rc.Width = width;
    rc.Height = height;

    Call(pDecoder->Copy(pDecoder, &rc, image_buffer, width * 4));
    ret.assign(image_buffer, image_buffer + buf_size);
    free(image_buffer);

    return ret;
  Cleanup:
    throw FormatError("ERROR: Could not get image bytes");
  }

} // namespace jxrlib
