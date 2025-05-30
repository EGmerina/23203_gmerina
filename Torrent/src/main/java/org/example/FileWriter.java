package org.example;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;

public class FileWriter {
    private final RandomAccessFile fileWriter;
    private final FileChannel fileChannel;

    public FileWriter(String filePath, long totalSize) throws IOException {
        this.fileWriter = new RandomAccessFile(filePath, "rw");
        this.fileChannel = fileWriter.getChannel();

        if (fileChannel.size() < totalSize) {
            fileChannel.truncate(totalSize);
        }
    }

    public void writePiece(int index, byte[] pieceData) throws IOException {
        long position = (long) index * pieceData.length;

        ByteBuffer buffer = ByteBuffer.wrap(pieceData);
        fileChannel.write(buffer, position);

        System.out.printf("piece %d write on disk %n", index);
    }
}
