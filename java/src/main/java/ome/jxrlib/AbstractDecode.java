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

abstract class AbstractDecode {

    public ImageMetadata getImageMetadata(byte[] source) {
        return new DecodeContext().getImageMetadata(source);
    }

    public ImageMetadata getImageMetadata(ByteBuffer source) {
        return new DecodeContext().getImageMetadata(source, 0, source.capacity());
    }

    public ImageMetadata getImageMetadata(ByteBuffer source, int offset, int length) {
        assert (source.capacity() >= offset + length);
        return new DecodeContext().getImageMetadata(source, offset, length);
    }

    public ImageMetadata getImageMetadata(File source) {
        return new DecodeContext().getImageMetadata(source.getAbsolutePath());
    }

    public byte[] decodeFrame(int frame, byte[] source) {
        return new DecodeContext().decodeFrame(frame, source, 0, source.length);
    }

    public byte[] decodeFrame(
            int frame, byte[] source, int offset, int length) {
        return new DecodeContext().decodeFrame(frame, source, offset, length);
    }

    public byte[] decodeFrame(int frame, File source) {
        return new DecodeContext().decodeFrame(frame, source.getAbsolutePath());
    }

    public void decodeFrame(
            int frame, ByteBuffer source, int sourceOffset, int sourceLength,
            ByteBuffer destination, int destinationOffset) {
        new DecodeContext().decodeFrame(
            frame, source, sourceOffset, sourceLength,
            destination, destinationOffset
        );
    }

    public void decodeFrame(
            int frame, File source, int offset, ByteBuffer destination) {
        new DecodeContext().decodeFrame(frame, source.getAbsolutePath(),
                                        offset, destination);
    }

    public void decodeFrame(int frame, File source, File destination) {
        new DecodeContext().decodeFrame(frame, source.getAbsolutePath(),
                                        destination.getAbsolutePath());
    }

}
