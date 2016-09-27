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
import java.io.IOException;
import java.io.RandomAccessFile;
import java.util.Vector;

public class Decode implements Closeable {

    static {
        System.loadLibrary("jxrjava");
    }

    private static final Factory factory = new Factory();
    private static final CodecFactory codecFactory = new CodecFactory();

    private final File inputFile;
    private final byte data[];
    private final ImageDecoder decoder;
    private final long frameCount;

    public Decode(File inputFile) {
        this.inputFile = inputFile;
        this.data = null;
        decoder = codecFactory.decoderFromFile(inputFile);
        frameCount = decoder.getFrameCount();
    }

    public Decode(byte data[]) {
        this.inputFile = null;
        this.data = data;
        decoder = codecFactory.decoderFromBytes(data);
        frameCount = decoder.getFrameCount();
    }

    public byte[] toBytes() {
        ByteArrayOutputStream decodedBytes = new ByteArrayOutputStream();
        for (long i = 0 ; i < frameCount ; i++) {
            decoder.selectFrame(i);
            ImageData data = decoder.getRawBytes();
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

    public static void printBytes(byte imageBytes[]) {
        for (int i = 0 ; i < imageBytes.length ; i += 4) {
            System.err.print(String.format("0x%02x%02x%02x%02x%s",
                                           imageBytes[i], imageBytes[i + 1], imageBytes[i + 2], imageBytes[i + 3],
                                           (i + 4) % 40 == 0 ? "\n" : " "));
        }
    }

    public void close() {
        if (decoder != null) {
            decoder.close();
        }
    }

    public static void main(String args[]) {
        Decode decode;

        if (args.length == 0) {
            ByteArrayOutputStream readData = new ByteArrayOutputStream();
            byte[] buffer = new byte[32 * 1024];

            int bytesRead;
            try {
                while ((bytesRead = System.in.read(buffer)) > 0) {
                    readData.write(buffer, 0, bytesRead);
                }
                byte[] bytes = readData.toByteArray();

                decode = new Decode(bytes);
                System.err.println("Opened decoder for bytes...");
                byte[] imageBytes = decode.toBytes();
                System.err.println("Decoded bytes:");
                printBytes(imageBytes);
            } catch (IOException e) {
                System.err.println("Problem parsing input data! " + e.getMessage());
            }
        } else if (args[0].equals("--in-memory")) {
          try {
            System.err.println("input file = " + args[1]);
            RandomAccessFile inputFile = new RandomAccessFile(args[1], "r");
            byte[] inputBuffer = new byte[(int) inputFile.length()];
            inputFile.readFully(inputBuffer);
            inputFile.close();

            decode = new Decode(inputBuffer);
            System.err.println("Opened in-memory decoder for file: " + args[1]);
            if (args.length == 2) {
                byte[] imageBytes = decode.toBytes();
                System.err.println("Decoded bytes:");
                printBytes(imageBytes);
            } else if (args.length == 3) {
                decode.toFile(new File(args[2]));
            } else {
                System.err.println("INVALID DECODE COMMAND");
            }
            }
            catch (IOException e) {
              System.err.println(e.getMessage());
            }
        } else {
            File inputFile = new File(args[0]);

            decode = new Decode(inputFile);
            System.err.println("Opened decoder for file: " + inputFile);
            if (args.length == 1) {
                byte[] imageBytes = decode.toBytes();
                System.err.println("Decoded bytes:");
                printBytes(imageBytes);
            } else if (args.length == 2) {
                decode.toFile(new File(args[1]));
            } else {
                System.err.println("INVALID DECODE COMMAND");
            }
        }
    }

}
