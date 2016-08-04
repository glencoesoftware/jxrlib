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

public class Decode {

    static {
        System.loadLibrary("jxr");
    }

    public static void main(String args[]) {
        try {
                Factory factory = new Factory();
                CodecFactory codecFactory = new CodecFactory();

                String inputFile = args[0];
                ImageDecoder decoder = codecFactory.decoderFromFile(inputFile);
                System.out.println("Opened decoder for file: " + inputFile);

                long frameCount = decoder.getFrameCount();
                System.out.println("Found " + frameCount + " frame" + (frameCount > 1 ? "s" : ""));

                String outputFile = args[1];
                String extension = outputFile.substring(outputFile.lastIndexOf('.') + 1);

                for (long i = 0 ; i < frameCount ; i++) {
                    decoder.selectFrame(i);
                    FormatConverter converter = codecFactory.createFormatConverter(decoder, extension);
                    System.out.println("Created format converter for extension: " + extension);
                    Stream outputStream = factory.createStreamFromFilename(outputFile);
                    System.out.println("Created output stream for file: " + outputFile);
                    ImageEncoder encoder = new ImageEncoder(outputStream, "." + extension);
                    System.out.println("Created image encoder");
                    encoder.initializeWithDecoder(decoder);
                    encoder.writeSource(converter);
                    encoder.close();
                }
                decoder.close();
            }
        catch (FormatError e) {
            System.out.println("*** ERROR: Unsupported format in JPEG XR ***\n" + e.getMessage());
        }
    }

}
