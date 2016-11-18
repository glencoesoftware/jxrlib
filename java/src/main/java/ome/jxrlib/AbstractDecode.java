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
        DecodeContext decodeContext = new DecodeContext();
        return decodeContext.getImageMetadata(source);
    }

    public byte[] decodeFrame(
            int frame, byte[] source, int offset, int length) {
        DecodeContext decodeContext = new DecodeContext();
        return decodeContext.decodeFrame(frame, source, offset, length);
    }

    public void decodeFrame(
            int frame, ByteBuffer source, int sourceOffset, int sourceLength,
            ByteBuffer destination, int destinationOffset) {
        DecodeContext decodeContext = new DecodeContext();
        decodeContext.decodeFrame(
            frame, source, sourceOffset, sourceLength,
            destination, destinationOffset);
    }

    public byte[] decodeFrame(
        int frame, File source) {
        DecodeContext decodeContext = new DecodeContext();
        return decodeContext.decodeFrame(
            frame, source.getAbsolutePath());
    }

    public void decodeFrame(
            int frame, File source, ByteBuffer destination,
            int offset, int length) {
        DecodeContext decodeContext = new DecodeContext();
        return decodeContext.decodeFrame(
            frame, source.getAbsolutePath(), destination);
    }

    public void decodeFrame(
        int frame, File source, File destination) {
        DecodeContext decodeContext = new DecodeContext();
        return decodeContext.decodeFrame(
            frame, source.getAbsolutePath(), destination.getAbsolutePath());
    }

}
