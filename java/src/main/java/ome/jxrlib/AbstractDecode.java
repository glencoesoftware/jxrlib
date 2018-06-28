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
 *  The AbstractDecode class implements a series of methods to decode JPEG-XR
 *  encoded images from files or data in memory, but does not specify how to
 *  load or where to locate the actual JPEG-XR C library and C++ wrappers. This
 *  detail is left to the concrete subclasses of AbstractDecode.
 */
abstract class AbstractDecode {

    private void checkBuffer(ByteBuffer buffer) throws DecodeException {
        if (!buffer.isDirect()) {
            throw new DecodeException("ByteBuffers must be allocated direct!");
        }
    }
    
    public void decodeFrame(int frame, File source, File destination) {
        new DecodeContext()
            .transcodeFile(frame, source.getAbsolutePath(),
                           destination.getAbsolutePath());
    }

    public void decodeFrame(int frame, File source, int sourceOffset,
                            File destination) {
        new DecodeContext()
            .transcodeFile(frame, source.getAbsolutePath(),
                          destination.getAbsolutePath(), sourceOffset);
    }

    public byte[] decodeFrame(int frame, File source) {
        return new DecodeContext().decodeFile(frame, source.getAbsolutePath());
    }

    public byte[] decodeFrame(int frame, File source, int sourceOffset) {
        return new DecodeContext()
            .decodeFile(frame, source.getAbsolutePath(), sourceOffset);
    }

    public void decodeFrame(int frame, File source, ByteBuffer destination)
            throws DecodeException {
        checkBuffer(destination);
        new DecodeContext()
            .decodeFileToBuffer(frame, source.getAbsolutePath(), destination);
    }

    public void decodeFrame(int frame, File source, int sourceOffset,
                            ByteBuffer destination) throws DecodeException {
        checkBuffer(destination);
        new DecodeContext()
            .decodeFileToBuffer(frame, source.getAbsolutePath(), destination,
                                sourceOffset);
    }

    public byte[] decodeFrame(int frame, byte[] source) {
        return new DecodeContext()
            .decodeBytes(frame, source, 0, source.length);
    }
    
    public byte[] decodeFrame(int frame, byte[] source,
                              int sourceOffset, int sourceLength) {
        assert (source.length >= sourceOffset + sourceLength);
        return new DecodeContext()
            .decodeBytes(frame, source, sourceOffset, sourceLength);
    }

    public byte[] decodeFrame(int frame, ByteBuffer source)
            throws DecodeException {
        checkBuffer(source);
        return new DecodeContext()
            .decodeBuffer(frame, source, 0, source.capacity());
    }
    
    public byte[] decodeFrame(int frame, ByteBuffer source,
                              int sourceOffset, int sourceLength)
            throws DecodeException {
        checkBuffer(source);
        assert (source.capacity() >= sourceOffset + sourceLength);
        return new DecodeContext()
            .decodeBuffer(frame, source, sourceOffset, sourceLength);
    }

    public void decodeFrame(int frame, byte[] source,
                            int sourceOffset, int sourceLength,
                            ByteBuffer destination)
            throws DecodeException {
        checkBuffer(destination);
        assert (source.length >= sourceOffset + sourceLength);
        new DecodeContext().decodeBytesToBuffer(frame, source, destination,
                                                sourceOffset, sourceLength);
    }

    public void decodeFrame(int frame, byte[] source,
                            int sourceOffset, int sourceLength,
                            ByteBuffer destination, int destinationOffset)
            throws DecodeException {
        checkBuffer(destination);
        assert (source.length >= sourceOffset + sourceLength);
        assert (destinationOffset < destination.capacity());
        new DecodeContext().decodeBytesToBuffer(frame, source, destination,
                                                sourceOffset, sourceLength,
                                                destinationOffset);
    }

    public void decodeFrame(int frame, ByteBuffer source,
                            int sourceOffset, int sourceLength,
                            ByteBuffer destination)
            throws DecodeException {
        checkBuffer(source);
        checkBuffer(destination);
        assert (source.capacity() >= sourceOffset + sourceLength);
        new DecodeContext().decodeBufferToBuffer(frame, source, destination,
                                                 sourceOffset, sourceLength);
    }

    public void decodeFrame(int frame, ByteBuffer source,
                            int sourceOffset, int sourceLength,
                            ByteBuffer destination, int destinationOffset)
            throws DecodeException {
        checkBuffer(source);
        checkBuffer(destination);
        assert (source.capacity() >= sourceOffset + sourceLength);
        assert (destinationOffset < destination.capacity());
        new DecodeContext().decodeBufferToBuffer(frame, source, destination,
                                                 sourceOffset, sourceLength,
                                                 destinationOffset);
    }

    public ImageMetadata getImageMetadata(File source) {
        return new DecodeContext()
            .getImageMetadataFromFile(source.getAbsolutePath());
    }

    public ImageMetadata getImageMetadata(File source, int sourceOffset) {
        return new DecodeContext()
            .getImageMetadataFromFile(source.getAbsolutePath(), sourceOffset);
    }

    public ImageMetadata getImageMetadata(byte[] source) {
        return new DecodeContext()
            .getImageMetadataFromBytes(source, 0, source.length);
    }

    public ImageMetadata getImageMetadata(byte[] source,
                                          int sourceOffset, int sourceLength) {
        return new DecodeContext()
            .getImageMetadataFromBytes(source, sourceOffset, sourceLength);
    }

    public ImageMetadata getImageMetadata(ByteBuffer source) {
        return new DecodeContext()
            .getImageMetadataFromBuffer(source, 0, source.capacity());
    }

    public ImageMetadata getImageMetadata(ByteBuffer source,
                                          int sourceOffset, int sourceLength) {
        assert (source.capacity() >= sourceOffset + sourceLength);
        return new DecodeContext()
            .getImageMetadataFromBuffer(source, sourceOffset, sourceLength);
    }

}
