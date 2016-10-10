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

#include "ImageEncoder.hpp"

#include "FormatError.hpp"
#include "ImageDecoder.hpp"
#include "JXRTest.h"
#include "Resolution.hpp"

namespace jxrlib {

  ImageEncoder::ImageEncoder(Stream encodeStream, std::string extension)
    : pEncoder(NULL), err(WMP_errSuccess) {
    const PKIID *pIID = NULL;
    Call(GetTestEncodeIID(extension.c_str(), &pIID));
    Call(PKTestFactory_CreateCodec(pIID, (void **)&pEncoder));
    Call(pEncoder->Initialize(pEncoder, encodeStream.pStream, NULL, 0));
    return;
  Cleanup:
    throw FormatError("ERROR: Unable to initialize encoder.");
  }

  void ImageEncoder::initializeWithDecoder(ImageDecoder& decoder) {
    Resolution decoderRes;
    // Set pixel properties
    Call(pEncoder->SetPixelFormat(pEncoder, decoder.getGUIDPixFormat()));
    pEncoder->WMP.wmiSCP.bBlackWhite = decoder.getBlackWhite();

    // Set size
    Call(pEncoder->SetSize(
		pEncoder, (I32)decoder.getWidth(), (I32)decoder.getHeight()));
    decoderRes = decoder.getResolution();
    Call(pEncoder->SetResolution(pEncoder, decoderRes.X, decoderRes.Y));
    return;
  Cleanup:
    throw FormatError("ERROR: Could not initialize encoder with settings from decoder");
  }

  void ImageEncoder::writeSource(FormatConverter& converter) {
    PKRect rect = { 0, 0, 0, 0};
    rect.Width = pEncoder->uWidth;
    rect.Height = pEncoder->uHeight;
    pEncoder->WriteSource = PKImageEncode_Transcode;
    Call(pEncoder->WriteSource(pEncoder, converter.pConverter, &rect));
    return;
  Cleanup:
    throw FormatError("ERROR: Could not write source");
  }

  void ImageEncoder::close() {
    pEncoder->Release(&pEncoder);
  }

} // namespace jxrlib
