package org.example;

import java.io.File;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;


public class PieceReader {
    private final long pieceLength;
    private final RandomAccessFile randomAccessFile;
    private final FileChannel fileChannel;

    public PieceReader(String filePath, long pieceLength) throws Exception {
        File file = new File(filePath);
        this.pieceLength = pieceLength;

        this.randomAccessFile = new RandomAccessFile(file, "r");
        this.fileChannel = randomAccessFile.getChannel();

    }

    public ByteBuffer readBlock(int index, int begin, int length) throws Exception {
        long position = (long) index * pieceLength + begin;

        if (fileChannel.size() - position > 0 && position + length > fileChannel.size()) {
            ByteBuffer buffer = ByteBuffer.allocate((int) (fileChannel.size() - position));
            fileChannel.read(buffer, position);
            buffer.flip();
            return buffer;
        }

        if (position > fileChannel.size()) {
            ByteBuffer buffer = ByteBuffer.allocate(0);
            buffer.flip();
            return buffer;
            //throw new IllegalArgumentException("Запрошенный блок выходит за пределы файла");
        }

        ByteBuffer buffer = ByteBuffer.allocate(length);
        fileChannel.read(buffer, position);
        buffer.flip();

        return buffer;
    }

    public void close() throws IOException {
        fileChannel.close();
        randomAccessFile.close();
    }
}