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
import java.io.Closeable;
import java.io.File;
import java.nio.ByteBuffer;

abstract class AbstractDecode implements Closeable {

    private final Factory factory = new Factory();
    private final CodecFactory codecFactory = new CodecFactory();

    private final File inputFile;
    private final byte data[];
    private final ImageDecoder decoder;
    private final long frameCount;

    public AbstractDecode(File inputFile) {
        this.inputFile = inputFile;
        this.data = null;
        decoder = codecFactory.decoderFromFile(inputFile);
        frameCount = decoder.getFrameCount();
    }

    public AbstractDecode(byte data[]) {
        this.inputFile = null;
        this.data = data;
        ByteBuffer buffer = ByteBuffer.allocateDirect(data.length);
        buffer.put(data);
        decoder = codecFactory.decoderFromBytes(buffer, data.length);
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

    public byte[] toBytes() {
        ByteArrayOutputStream decodedBytes = new ByteArrayOutputStream();
        for (long i = 0 ; i < frameCount ; i++) {
            decoder.selectFrame(i);
            ImageData data = decoder.getRawBytes();
            decodedBytes = new ByteArrayOutputStream((int)data.size());
            for (int j = 0 ; j < data.size() ; j++) {
                decodedBytes.write(data.get(j));
            }
        }
        return decodedBytes.toByteArray();
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

    public void close() {
        if (decoder != null) {
            decoder.close();
        }
    }

}
