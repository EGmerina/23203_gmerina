package org.example;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.nio.ByteBuffer;
import java.security.MessageDigest;
import java.util.Arrays;
import java.util.BitSet;
import java.util.HashMap;
import java.util.Map;

public class PieceAssembler {
    private static final Logger logger = LogManager.getLogger(PieceAssembler.class);

    private final int pieceLength;
    private final int blockSize = 1024 * 16;
    private final Map<Integer, byte[]> receivedPieces = new HashMap<>();
    private final Map<Integer, BitSet> receivedBlocks = new HashMap<>();
    private final Map<Integer, Integer> realSizesOfPieces = new HashMap<>();
    private final MessageDigest messageDigest;

    public PieceAssembler(int pieceLength) throws Exception {
        this.pieceLength = pieceLength;
        this.messageDigest = MessageDigest.getInstance("SHA-1");
    }

    public void addBlock(int index, int begin, byte[] data) {
        if (!receivedPieces.containsKey(index)) {
            receivedPieces.put(index, new byte[pieceLength]);
            receivedBlocks.put(index, new BitSet());
            realSizesOfPieces.put(index, 0);
        }

        byte[] pieceData = receivedPieces.get(index);
        realSizesOfPieces.put(index, realSizesOfPieces.get(index) + data.length);
        System.arraycopy(data, 0, pieceData, begin, data.length);
        receivedBlocks.get(index).set(begin / blockSize);

    }

    public boolean isPieceComplete(int index) {
        int blocksInPiece = (int) Math.ceil((double) pieceLength / blockSize);
        return receivedBlocks.getOrDefault(index, new BitSet()).cardinality() == blocksInPiece;
    }

    public byte[] getAssembledPiece(int index) {
        if (isPieceComplete(index)) {
            byte[] data = receivedPieces.get(index);
            return Arrays.copyOf(data, realSizesOfPieces.get(index));

        }
        return null;
    }

    public boolean validatePieceHash(int index, byte[] pieceData, byte[] pieceHash) {
        byte[] hash = messageDigest.digest(pieceData);

        boolean isValid = Arrays.equals(hash, pieceHash);
        if (isValid) {
            logger.info("hash is correct: " + index);
        } else {
            logger.error("hash is not correct: " + index);
        }
        return isValid;
    }

    public void removePiece(int index) {
        receivedPieces.remove(index);
        receivedBlocks.remove(index);
    }
}
