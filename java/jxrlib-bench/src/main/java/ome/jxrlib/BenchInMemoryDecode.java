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
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.concurrent.TimeUnit;

import org.openjdk.jmh.annotations.Benchmark;
import org.openjdk.jmh.annotations.BenchmarkMode;
import org.openjdk.jmh.annotations.Measurement;
import org.openjdk.jmh.annotations.Mode;
import org.openjdk.jmh.annotations.OutputTimeUnit;
import org.openjdk.jmh.annotations.Param;
import org.openjdk.jmh.annotations.Scope;
import org.openjdk.jmh.annotations.Setup;
import org.openjdk.jmh.annotations.State;

@BenchmarkMode(Mode.AverageTime)
@OutputTimeUnit(TimeUnit.SECONDS)
@Measurement(iterations = 3)
@State(Scope.Benchmark)
public class BenchInMemoryDecode {
    @Param("BF-1scene-nozstack-compression85pct.jxr")
    public String filename;

    @Benchmark
    public void benchArrayDecode() throws IOException, DecodeException {
        byte[] imageData;
        Decode byteArrayDecode;
        ByteBuffer decodedData;

        imageData = Files.readAllBytes(Paths.get(filename));

        byteArrayDecode = new Decode(imageData);
        decodedData = ByteBuffer.allocateDirect(
                (int) (byteArrayDecode.getWidth() *
                        byteArrayDecode.getHeight() *
                        byteArrayDecode.getBytesPerPixel()));
        byteArrayDecode.toBytes(decodedData);
    }

}
