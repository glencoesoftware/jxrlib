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

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.URISyntaxException;
import java.net.URL;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.file.Path;
import java.nio.file.Paths;

import org.testng.Assert;
import org.testng.annotations.AfterMethod;
import org.testng.annotations.Parameters;
import org.testng.annotations.Test;

public class TestInMemoryDecode extends AbstractTest {

    void assertDecode(
            AbstractDecode decode, long width, long height, long bpp,
            String md5) throws DecodeException {
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

    byte[] asByteArray(String filename) throws IOException {
        InputStream stream =
            this.getClass().getClassLoader().getResourceAsStream(filename);
        ByteArrayOutputStream outputStream = new ByteArrayOutputStream();

        byte[] buffer = new byte[1024 * 1024];
        int rlen = 0;
        while ((rlen = stream.read(buffer)) != -1) {
            outputStream.write(buffer, 0, rlen);
        }

        return outputStream.toByteArray();
    }

    ByteBuffer asByteBuffer(String filename)
            throws URISyntaxException, IOException {
        URL url = this.getClass().getClassLoader().getResource(filename);
        Path inputFile = Paths.get(url.toURI());

        ByteBuffer dataBuffer;
        try (FileChannel channel = FileChannel.open(inputFile)) {
            dataBuffer = ByteBuffer.allocateDirect((int)channel.size());
            channel.read(dataBuffer);
            dataBuffer.position(0);
        }
        return dataBuffer;
    }

    @Parameters({"filename", "width", "height", "bpp", "md5"})
    @Test
    public void testByteArray(
        String filename, long width, long height, long bpp, String md5)
            throws IOException, DecodeException {
        byte[] data = asByteArray(filename);

        TestDecode decode = new TestDecode(data);
        assertDecode(decode, width, height, bpp, md5);
    }

    @Parameters({"filename", "width", "height", "bpp", "md5"})
    @Test
    public void testByteBuffer(
        String filename, long width, long height, long bpp, String md5)
            throws IOException, URISyntaxException, DecodeException {
        ByteBuffer dataBuffer = asByteBuffer(filename);
        TestDecode decode = new TestDecode(dataBuffer);
        assertDecode(decode, width, height, bpp, md5);
    }

    @Test(expectedExceptions={DecodeException.class})
    public void testInputNotDirect() throws DecodeException {
        ByteBuffer dataBuffer = ByteBuffer.allocate(1);
        new TestDecode(dataBuffer);
    }

    @Parameters({"filename"})
    @Test(expectedExceptions={DecodeException.class})
    public void testOutputNotDirect(String filename)
            throws IOException, URISyntaxException, DecodeException {
        ByteBuffer dataBuffer = asByteBuffer(filename);
        TestDecode decode = new TestDecode(dataBuffer);
        decode.toBytes(ByteBuffer.allocate(1));
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
