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

public class TestDecode extends AbstractDecode {

    static {
        System.loadLibrary("jxrjava");
    }

    public TestDecode(File inputFile) {
        super(inputFile);
    }

    public TestDecode(byte data[]) throws DecodeException {
        super(data);
    }

    public TestDecode(ByteBuffer dataBuffer) throws DecodeException {
        super(dataBuffer);
    }

    public TestDecode(ByteBuffer dataBuffer, int offset, int length)
            throws DecodeException {
        super(dataBuffer, offset, length);
    }

    public static byte[] decodeFirstFrame(
            byte[] source, int offset, int length, int size) {
        return AbstractDecode.decodeFirstFrame(source, offset, length, size);
    }
}
