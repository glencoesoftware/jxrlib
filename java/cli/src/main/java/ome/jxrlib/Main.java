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
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.file.Paths;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.ArrayList;
import java.util.List;

import javax.xml.bind.DatatypeConverter;

import org.kohsuke.args4j.Argument;
import org.kohsuke.args4j.CmdLineException;
import org.kohsuke.args4j.CmdLineParser;
import org.kohsuke.args4j.Option;
import org.slf4j.LoggerFactory;

import ch.qos.logback.classic.Level;
import ch.qos.logback.classic.Logger;

public class Main {

    static String md5(ByteBuffer byteBuffer) {
        byte[] bytes = new byte[byteBuffer.capacity()];
        byteBuffer.get(bytes);
        return md5(bytes);
    }

    static String md5(byte[] bytes) {
        MessageDigest md;
        try {
            md = MessageDigest.getInstance("MD5");
        } catch (NoSuchAlgorithmException e) {
            // This should never happen
            throw new RuntimeException(e);
        }
        return DatatypeConverter.printHexBinary(md.digest(bytes)).toLowerCase();
    }

    @Option(name="--debug")
    private boolean debug;

    @Option(name="--in-memory")
    private boolean inMemory;

    @Option(name="--in-memory-bytes")
    private boolean inMemoryBytes;

    @Argument
    private List<String> arguments = new ArrayList<String>();

    public static void main(String args[])
            throws DecodeException, IOException, CmdLineException {
      new Main().go(args);
    }

    public void go(String args[])
            throws DecodeException, IOException, CmdLineException {
        CmdLineParser parser = new CmdLineParser(this);
        try {
            parser.parseArgument(args);
        } catch (CmdLineException e) {
            System.err.println(e.getMessage());
            System.err.println("cli [options...] arguments...");
            parser.printUsage(System.err);
            System.err.println();

            return;
        }

        Decode decode;
        ImageMetadata metadata;

        if (debug) {
            Logger root =
                (Logger) LoggerFactory.getLogger(Logger.ROOT_LOGGER_NAME);
            root.setLevel(Level.DEBUG);
        }

        if (arguments.isEmpty()) {
            ByteArrayOutputStream readData = new ByteArrayOutputStream();
            byte[] buffer = new byte[32 * 1024];

            int bytesRead;
            while ((bytesRead = System.in.read(buffer)) > 0) {
                readData.write(buffer, 0, bytesRead);
            }
            byte[] bytes = readData.toByteArray();

            decode = new Decode();
            System.err.println("Opened decoder for bytes...");
            byte[] imageData = decode.decodeFrame(0, bytes, 0, bytes.length);
            System.err.println("Decoded bytes MD5: " + md5(imageData));
        } else if (inMemory || inMemoryBytes) {
            String inputFilename = arguments.get(0);
            System.err.println("input file = " + inputFilename);
            ByteBuffer inputBuffer;
            try (FileChannel channel = FileChannel.open(
                    Paths.get(inputFilename))) {
              inputBuffer = ByteBuffer.allocateDirect((int) channel.size());
              channel.read(inputBuffer);
              channel.position(0);
            }

            System.err.println(
                "Opened in-memory decoder for file: " + inputFilename);
            if (inMemoryBytes) {
                System.err.println("Decoding using byte arrays");
                byte[] source = new byte[inputBuffer.capacity()];
                inputBuffer.position(0);
                inputBuffer.get(source);
                byte[] destination = new Decode().decodeFrame(0, source,
                                                              0, source.length);
                System.err.println(
                        "Decoded bytes MD5: " + md5(destination));
                return;
            }
            decode = new Decode();
            if (arguments.size() == 1) {
                metadata = decode.getImageMetadata(inputBuffer.array());
                long width = metadata.getWidth();
                long height = metadata.getHeight();
                long bpp = metadata.getBytesPerPixel();
                System.err.println("Decoding using NIO byte buffers");
                ByteBuffer imageBuffer = ByteBuffer.allocateDirect(
                  (int) (width * height * bpp));
                decode.decodeFrame(0, inputBuffer, 0, inputBuffer.length,
                                   imageBuffer, 0);
                System.err.println(
                    "Decoded bytes MD5: " + md5(imageBuffer));
            } else if (arguments.size() == 2) {
                System.err.println("Not implemented in API v0.3.0");
            } else {
                throw new CmdLineException(
                    parser, "At least one argument required");
            }
        } else {
            String inputFilename = arguments.get(0);
            File inputFile = new File(inputFilename);

            decode = new Decode();
            metadata = decode.getImageMetadata(inputFile);
            long width = metadata.getWidth();
            long height = metadata.getHeight();
            long bpp = metadata.getBytesPerPixel();
            ByteBuffer imageBuffer = ByteBuffer.allocateDirect(
                (int) (width * height * bpp));

            System.err.println("Opened decoder for file: " + inputFilename);
            if (args.length == 1) {
                decode.decodeFrame(0, inputFile, 0, imageBuffer);
                System.err.println("Decoded bytes MD5: " + md5(imageBuffer));
            } else if (args.length == 2) {
                decode.decodeFrame(0, inputFile, new File(args[1]));
            } else {
                System.err.println("INVALID DECODE COMMAND");
            }
        }
    }

}
