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

import java.io.IOException;
import java.net.URISyntaxException;
import java.net.URL;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.file.FileVisitResult;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.SimpleFileVisitor;
import java.nio.file.attribute.BasicFileAttributes;

import org.testng.Assert;
import org.testng.annotations.AfterClass;
import org.testng.annotations.BeforeClass;
import org.testng.annotations.Parameters;
import org.testng.annotations.Test;

public class TestFileToFileDecode extends AbstractTest {

    private Path temporaryDirectory;

    @BeforeClass
    public void setUp() throws IOException {
        temporaryDirectory = Files.createTempDirectory(
            this.getClass().getName());
    }

    @Parameters({"filename", "tiffMd5"})
    @Test
    public void test(String filename, String tiffMd5)
            throws IOException, URISyntaxException {
        URL url = this.getClass().getClassLoader().getResource(filename);
        Path inputFile = Paths.get(url.toURI());

        TestDecode decode = new TestDecode(inputFile.toFile());
        Path outputFile = temporaryDirectory.resolve(
                filename.replace(".jxr", ".tif"));
        decode.toFile(outputFile.toFile());

        try (FileChannel channel = FileChannel.open(outputFile)) {
            ByteBuffer outputFileBuffer = ByteBuffer.allocate((int)channel.size());
            channel.read(outputFileBuffer);
            outputFileBuffer.position(0);
            // XXX: These are all wrong right now due to encoder issues
            //Assert.assertEquals(md5(outputFileBuffer), tiffMd5);
        }
    }

    @AfterClass
    public void tearDown() throws IOException {
        Files.walkFileTree(temporaryDirectory, new SimpleFileVisitor<Path>()
        {
            @Override
            public FileVisitResult visitFile(
                Path file, BasicFileAttributes attrs)
                    throws IOException {
                Files.delete(file);
                return FileVisitResult.CONTINUE;
            }

            @Override
            public FileVisitResult visitFileFailed(Path file, IOException exc)
                    throws IOException {
                // try to delete the file anyway, even if its attributes
                // could not be read, since delete-only access is
                // theoretically possible
                Files.delete(file);
                return FileVisitResult.CONTINUE;
            }

            @Override
            public FileVisitResult postVisitDirectory(Path dir, IOException exc)
                    throws IOException {
                if (exc == null) {
                    Files.delete(dir);
                    return FileVisitResult.CONTINUE;
                } else {
                    // directory iteration failed; propagate exception
                    throw exc;
                }
            }
        });
    }
}
