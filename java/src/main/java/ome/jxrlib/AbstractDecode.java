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

    private final Factory factory = new Factory();
    private final CodecFactory codecFactory = new CodecFactory();

    private final File inputFile;
    private final ByteBuffer dataBuffer;
    private final ImageDecoder decoder;
    private final long frameCount;

    public AbstractDecode(File inputFile) {
        this.inputFile = inputFile;
        this.dataBuffer = null;
        decoder = new ImageDecoder();
        codecFactory.decoderFromFile(decoder, inputFile.getAbsolutePath());
        frameCount = decoder.getFrameCount();
    }

    public AbstractDecode(byte data[]) throws DecodeException {
        this(ByteBuffer.allocateDirect(data.length).put(data));
    }

    public AbstractDecode(ByteBuffer dataBuffer) throws DecodeException {
        if (!dataBuffer.isDirect()) {
            throw new DecodeException("Buffer must be allocated direct.");
        }
        this.inputFile = null;
        this.dataBuffer = dataBuffer;
        decoder = new ImageDecoder();
        codecFactory.decoderFromBytes(decoder, dataBuffer, dataBuffer.capacity());
        frameCount = decoder.getFrameCount();
    }

    public long getWidth() {
        return decoder.getWidth();
    }

    public long getHeight() {
        return decoder.getHeight();
    }

    public long getBytesPerPixel() {
        return decoder.getBytesPerPixel();
    }

    public void toBytes(ByteBuffer imageBuffer) {
        decoder.getRawBytes(imageBuffer);
    }

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

}
