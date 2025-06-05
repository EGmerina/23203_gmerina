package org.example;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;

public class FileWriter {
    private static final Logger logger = LogManager.getLogger(FileWriter.class);
    private final RandomAccessFile fileWriter;
    private final FileChannel fileChannel;
    private final long piecesLength;

    public FileWriter(String filePath, long totalSize, long piecesLength) throws IOException {
        this.fileWriter = new RandomAccessFile(filePath, "rw");
        this.fileChannel = fileWriter.getChannel();
        this.piecesLength = piecesLength;
        if (fileChannel.size() < totalSize) {
            fileChannel.truncate(totalSize);
        }
    }

    public void writePiece(int index, byte[] pieceData) throws IOException {
        long position = (long) index * piecesLength;

        ByteBuffer buffer = ByteBuffer.wrap(pieceData);
        fileChannel.write(buffer, position);

        logger.info("piece {} write on disk ({} bytes)", index, pieceData.length);
    }
}
