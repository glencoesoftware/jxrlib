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
import java.io.RandomAccessFile;

public class Main {

      public static void printBytes(byte imageBytes[]) {
          for (int i = 0 ; i < imageBytes.length ; i += 4) {
              byte a = i < imageBytes.length? imageBytes[i] : 0;
              byte b = i + 1 < imageBytes.length? imageBytes[i + 1] : 0;
              byte c = i + 2 < imageBytes.length? imageBytes[i + 2] : 0;
              byte d = i + 3 < imageBytes.length? imageBytes[i + 3] : 0;
              System.err.print(String.format(
                  "0x%02x%02x%02x%02x%s",
                  a, b, c, d,
                  (i + 4) % 40 == 0 ? "\n" : " "));
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
