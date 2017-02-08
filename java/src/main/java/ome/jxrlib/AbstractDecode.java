/*
 * Copyright (C) 2106 Glencoe Software, Inc. All rights reserved.
 *
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
 */

package ome.jxrlib;

import java.io.File;
import java.nio.ByteBuffer;

/**
 *  The AbstractDecode class implements the bulk of the methods needed to decode
 *  JPEG-XR encoded images, but does not specify how to load or where to locate
 *  the actual JPEG-XR C library and C++ wrappers. This detail is left to the
 *  concrete subclasses of AbstractDecode.
 */
abstract class AbstractDecode {

    public ImageMetadata getImageMetadata(byte[] source) {
        return new DecodeContext().getImageMetadataFromBytes(source);
    }

    public ImageMetadata getImageMetadata(ByteBuffer source) {
        return new DecodeContext().getImageMetadataFromBuffer(source,
                                                              0, source.capacity());
    }

    public ImageMetadata getImageMetadata(ByteBuffer source, int offset, int length) {
        assert (source.capacity() >= offset + length);
        return new DecodeContext().getImageMetadataFromBuffer(source, offset, length);
    }

    public ImageMetadata getImageMetadata(File source) {
        return new DecodeContext().getImageMetadataFromFile(source.getAbsolutePath());
    }

    public byte[] decodeFrame(int frame, byte[] source) {
        return new DecodeContext().decodeBytes(frame, source,
                                               0, source.length);
    }

    public byte[] decodeFrame(
                              int frame, byte[] source, int offset, int length) {
        assert (source.length >= offset + length);
        return new DecodeContext().decodeBytes(frame, source, offset, length);
    }

    public byte[] decodeFrame(int frame, File source) {
        return new DecodeContext().decodeFile(frame, source.getAbsolutePath());
    }

    public void decodeFrame(
                            int frame, ByteBuffer source, int sourceOffset, int sourceLength,
                            ByteBuffer destination, int destinationOffset) {
        assert (source.capacity() >= sourceOffset + sourceLength);
        new DecodeContext().decodeBufferToBuffer(frame, source, destination,
                                                 sourceOffset, sourceLength,
                                                 destinationOffset);
    }

    public void decodeFrame(
                            int frame, File source, int sourceOffset, ByteBuffer destination) throws DecodeException {
        if (!destination.isDirect()) {
            throw new DecodeException("ByteBuffers must be allocated direct!");
        }
        new DecodeContext().decodeFileToBuffer(frame, source.getAbsolutePath(),
                                               destination, sourceOffset);
    }

    public void decodeFrame(int frame, File source, File destination) {
        new DecodeContext().transcodeFile(frame, source.getAbsolutePath(),
                                          destination.getAbsolutePath());
    }

}
