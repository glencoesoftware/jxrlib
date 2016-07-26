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
#include "JXRTest.h"

namespace jxrlib {

  ImageEncoder::ImageEncoder(Stream encodeStream, std::string extension)
    : pEncoder(NULL), err(WMP_errSuccess) {
    const PKIID *pIID = NULL;
    Call(GetTestEncodeIID(extension.c_str(), &pIID));
    Call(PKTestFactory_CreateCodec(pIID, (void **)&pEncoder));
    Call(pEncoder->Initialize(pEncoder, encodeStream.pEncodeStream, NULL, 0));
    return;
  Cleanup:
    throw FormatError("ERROR: Unable to initialize encoder.");
  }

  ImageEncoder::~ImageEncoder() {
    pEncoder->Release(&pEncoder);
  }

} // namespace jxrlib
