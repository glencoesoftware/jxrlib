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

import java.io.IOException;
import java.nio.ByteBuffer;

import org.testng.Assert;
import org.testng.annotations.AfterMethod;
import org.testng.annotations.Parameters;
import org.testng.annotations.Test;

public class TestInMemoryDecode extends AbstractTest {

    @Parameters({"filename", "width", "height", "bpp", "md5"})
    @Test
    public void test(
        String filename, long width, long height, long bpp, String md5)
            throws IOException {
        byte[] data = getData(filename);

        TestDecode decode = new TestDecode(data);
        long _width = decode.getWidth();
        Assert.assertEquals(_width, width);
        long _height = decode.getHeight();
        Assert.assertEquals(_height, height);
        long _bpp = decode.getBytesPerPixel();
        Assert.assertEquals(_bpp, bpp);

        ByteBuffer imageBuffer = ByteBuffer.allocateDirect(
            (int) (_width * _height * _bpp));
        decode.toBytes(imageBuffer);

        Assert.assertEquals(md5(imageBuffer), md5);
    }

    // Can be useful if debugging destructors.
    /*
    @AfterMethod
    public void cleanup() {
        System.err.println("Java cleanup.");
        System.gc();
        System.runFinalization();
    }
    */
}
