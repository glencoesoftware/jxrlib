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
import java.io.File;
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

    public static ByteBuffer readBuffer = null, imageBuffer = null;

    void assertDecode(byte[] inputData, long width, long height, long bpp,
            String md5) throws DecodeException {
        TestDecode decode = new TestDecode();
        ImageMetadata metadata = decode.getImageMetadata(inputData);
        long _width = metadata.getWidth();
        Assert.assertEquals(_width, width);
        long _height = metadata.getHeight();
        Assert.assertEquals(_height, height);
        long _bpp = metadata.getBytesPerPixel();
        Assert.assertEquals(_bpp, bpp);

        byte[] imageData = decode.decodeFrame(0, inputData);

        Assert.assertEquals(md5(imageData), md5);
    }

    void assertDecode(ByteBuffer inputBuffer, ImageMetadata metadata, long offset,
                      long length, long width, long height, long bpp,
                      String md5) throws DecodeException {
        long _width = metadata.getWidth();
        Assert.assertEquals(_width, width);
        long _height = metadata.getHeight();
        Assert.assertEquals(_height, height);
        long _bpp = metadata.getBytesPerPixel();
        Assert.assertEquals(_bpp, bpp);

        int size = (int)_width * (int)_height * (int)_bpp;
        if (imageBuffer == null || imageBuffer.capacity() < size) {
            imageBuffer = ByteBuffer.allocateDirect(size);
        }

        TestDecode decode = new TestDecode();
        decode.decodeFrame(0, inputBuffer, 0, size, imageBuffer, 0);
        byte[] imageData = new byte[size];
        imageBuffer.get(imageData);
        Assert.assertEquals(md5(imageData), md5);
    }

    void assertDecode(ByteBuffer inputBuffer, ImageMetadata metadata,
                      long width, long height, long bpp,
                      String md5) throws DecodeException {
        assertDecode(inputBuffer, metadata, 0, width * height * bpp,
                     width, height, bpp, md5);
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
        return asByteBuffer(filename, null, null);
    }

    ByteBuffer asByteBuffer(String filename, Integer offset, Integer length)
            throws URISyntaxException, IOException {
        URL url = this.getClass().getClassLoader().getResource(filename);
        Path inputFile = Paths.get(url.toURI());

        try (FileChannel channel = FileChannel.open(inputFile)) {
            if (offset == null) {
                offset = 0;
            }
            if (length == null) {
                length = (int) channel.size();
            }
            if (readBuffer == null || readBuffer.capacity() < offset + length) {
                readBuffer = ByteBuffer.allocateDirect(offset + length);
            }
            readBuffer.position(offset);
            channel.read(readBuffer);
            readBuffer.position(0);
        }
        return readBuffer;
    }

    @Parameters({"filename", "width", "height", "bpp", "md5"})
    @Test
    public void testByteArray(
        String filename, long width, long height, long bpp, String md5)
            throws IOException, DecodeException {
        byte[] data = asByteArray(filename);

        assertDecode(data, width, height, bpp, md5);
    }

    @Parameters({"filename", "md5"})
    @Test
    public void testFirstFrame(String filename, String md5)
            throws IOException, DecodeException {
        byte[] data = asByteArray(filename);
        byte[] destination = new TestDecode().decodeFrame(0, data);
        Assert.assertEquals(md5, md5(destination));
    }

    /*
    @Parameters({"filename", "width", "height", "bpp", "md5"})
    @Test
    public void testByteBuffer(
        String filename, long width, long height, long bpp, String md5)
            throws IOException, URISyntaxException, DecodeException {
        ByteBuffer dataBuffer = asByteBuffer(filename);

        URL url = this.getClass().getClassLoader().getResource(filename);
        Path inputFile = Paths.get(url.toURI());
        ImageMetadata metadata = new TestDecode().getImageMetadata(inputFile.toFile());

        assertDecode(dataBuffer, metadata, width, height, bpp, md5);
    }

    @Parameters({"filename", "width", "height", "bpp", "md5"})
    @Test
    public void testByteBufferOffsetLength(
        String filename, long width, long height, long bpp, String md5)
            throws IOException, URISyntaxException, DecodeException {
        int offset = 1024 * 1024;
        ByteBuffer dataBuffer = asByteBuffer(filename, offset, null);

        URL url = this.getClass().getClassLoader().getResource(filename);
        Path inputFile = Paths.get(url.toURI());
        ImageMetadata metadata = new TestDecode().getImageMetadata(inputFile.toFile());

        assertDecode(dataBuffer, metadata, offset, dataBuffer.capacity() - offset,
                     width, height, bpp, md5);
    }

    @Parameters({"filename"})
    @Test(expectedExceptions={DecodeException.class})
    public void testOutputNotDirect(String filename)
            throws IOException, URISyntaxException, DecodeException {
        ByteBuffer dataBuffer = asByteBuffer(filename);
        new TestDecode().decodeFrame(0, dataBuffer, 0, dataBuffer.capacity(),
                                     ByteBuffer.allocate(1), 0);
    }

    @Test(expectedExceptions={FormatError.class})
    public void testInvalidInput()
            throws IOException, URISyntaxException, DecodeException {
        new TestDecode().decodeFrame(0, ByteBuffer.allocateDirect(1).array(), 0, 1);
    }
    */

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
