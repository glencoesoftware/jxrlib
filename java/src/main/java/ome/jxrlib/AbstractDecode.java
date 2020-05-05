/*
 * Copyright (C) 2016-2020 Glencoe Software, Inc. All rights reserved.
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
 * Abstract class for decoding JPEG-XR data.
 * @see Decode
 */
abstract class AbstractDecode {

    private final Factory factory = new Factory();
    private final CodecFactory codecFactory = new CodecFactory();

    private final File inputFile;
    private final ByteBuffer dataBuffer;
    private final ImageDecoder decoder;
    private final long frameCount;

    /**
     * Construct a decoder for the JPEG-XR data in the given file.
     *
     * @param inputFile file containing JPEG-XR data.
     *        Should exist and be non-null.
     */
    public AbstractDecode(File inputFile) {
        this.inputFile = inputFile;
        this.dataBuffer = null;
        decoder = new ImageDecoder();
        codecFactory.decoderFromFile(decoder, inputFile.getAbsolutePath());
        frameCount = decoder.getFrameCount();
    }

    /**
     * Construct a decoder for the JPEG-XR data in the given byte array.
     *
     * @param data array containing JPEG-XR data; should be non-null
     */
    AbstractDecode(byte data[]) throws DecodeException {
        this(ByteBuffer.allocateDirect(data.length).put(data));
    }

    /**
     * Construct a decoder for the JPEG-XR data in the given buffer.
     *
     * @param dataBuffer buffer containing JPEG-XR data.
     *        Should be non-null and directly allocated.
     * @throws DecodeException if the buffer was not directly allocated
     */
    AbstractDecode(ByteBuffer dataBuffer) throws DecodeException {
        this(dataBuffer, 0, dataBuffer.capacity());
    }

    /**
     * Construct a decoder for the JPEG-XR data in the given buffer.
     *
     * @param dataBuffer buffer containing JPEG-XR data.
     *        Should be non-null and directly allocated.
     * @param offset starting offset within the given buffer
     * @param length number of bytes to read from the given buffer
     * @throws DecodeException if the buffer was not directly allocated
     */
    AbstractDecode(ByteBuffer dataBuffer, int offset, int length)
            throws DecodeException {
        if (!dataBuffer.isDirect()) {
            throw new DecodeException("Buffer must be allocated direct.");
        }
        this.inputFile = null;
        this.dataBuffer = dataBuffer;
        decoder = new ImageDecoder();
        codecFactory.decoderFromBytes(decoder, dataBuffer, offset, length);
        frameCount = decoder.getFrameCount();
    }

    /**
     * @return image width in pixels
     */
    public long getWidth() {
        return decoder.getWidth();
    }

    /**
     * @return image height in pixels
     */
    public long getHeight() {
        return decoder.getHeight();
    }

    /**
     * @return number of bytes per pixel, equal to the number of channels
     *         multiplied by the number of bytes per channel
     */
    public long getBytesPerPixel() {
        return decoder.getBytesPerPixel();
    }

    /**
     * @return the pixel format
     */
    public GUID getPixelFormat() {
        return decoder.getGUIDPixFormat();
    }

    /**
     * @return true if channels are returned in BGR order
     */
    public boolean isBGR() {
        return decoder.isBGR();
    }

    /**
     * Decode the current JPEG-XR data into the given buffer.
     *
     * @param imageBuffer directly allocated buffer to store decoded data
     * @throws DecodeException if the buffer is not directly allocated
     */
    public void toBytes(ByteBuffer imageBuffer) throws DecodeException {
        if (!imageBuffer.isDirect()) {
            throw new DecodeException("Buffer must be allocated direct.");
        }
        decoder.getRawBytes(imageBuffer);
    }

    /**
     * Transcode the current JPEG-XR data to a file in a supported output
     * format (e.g. BMP).
     * The file extension is used to determine the output format.
     *
     * @param outputFile location to write data,
     *        should be neither null nor an existing file
     */
    public void toFile(File outputFile) {
        String fileName = outputFile.getName();
        String extension = fileName.substring(fileName.lastIndexOf('.') + 1);

        for (long i = 0 ; i < frameCount ; i++) {
            decoder.selectFrame(i);
            FormatConverter converter = codecFactory.createFormatConverter(decoder, extension);
            System.err.println("Created format converter for extension: " + extension);
            Stream outputStream = factory.createStreamFromFilename(outputFile.getAbsolutePath());
            System.err.println("Created output stream for file: " + fileName);
            ImageEncoder encoder = new ImageEncoder(outputStream, "." + extension);
            System.err.println("Created image encoder");
            encoder.initializeWithDecoder(decoder);
            encoder.writeSource(converter);
            encoder.close();
        }
    }

    /**
     * Decode the first JPEG-XR frame from the given byte array.
     *
     * @param source byte array containing JPEG-XR data
     * @param offset starting index in the source byte array
     * @param length number of valid bytes in the array starting at offset
     * @return uncompressed data
     */
    protected static byte[] decodeFirstFrame(
            byte[] source, int offset, int length) {
        DecodeContext decodeContext = new DecodeContext();
        return decodeContext.decodeFirstFrame(source, offset, length);
    }
}
