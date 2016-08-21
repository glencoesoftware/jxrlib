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

#include "Factory.hpp"

#include "FormatError.hpp"
#include "windowsmediaphoto.h"

namespace jxrlib {

  Factory::Factory() : pFactory(NULL), err(WMP_errSuccess) {
    Call(PKCreateFactory(&pFactory, PK_SDK_VERSION));
    return;
  Cleanup:
    throw FormatError("ERROR: Unable to instantiate Factory.");
  }

  Stream Factory::createStreamFromFilename(std::string filename) {
    Stream fileStream;
    Call(pFactory->CreateStreamFromFilename(&fileStream.pStream,
                                            filename.c_str(),
                                            "wb"));
    return fileStream;
  Cleanup:
    std::string msg = "ERROR: Unable to create stream for file: " + filename;
    throw FormatError(msg);
  }

} // namespace jxrlib
