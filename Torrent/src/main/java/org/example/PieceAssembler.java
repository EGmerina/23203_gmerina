package org.example;

import java.nio.ByteBuffer;
import java.security.MessageDigest;
import java.util.Arrays;
import java.util.BitSet;
import java.util.HashMap;
import java.util.Map;

public class PieceAssembler {
    private final int pieceLength;
    private final int blockSize = 1024 * 16;
    private final Map<Integer, byte[]> receivedPieces = new HashMap<>();
    private final Map<Integer, BitSet> receivedBlocks = new HashMap<>();
    private final MessageDigest messageDigest;

    public PieceAssembler(int pieceLength) throws Exception {
        this.pieceLength = pieceLength;
        this.messageDigest = MessageDigest.getInstance("SHA-1");
    }

    public void addBlock(int index, int begin, byte[] data) {
        if (!receivedPieces.containsKey(index)) {
            receivedPieces.put(index, new byte[pieceLength]);
            receivedBlocks.put(index, new BitSet());
        }

        byte[] pieceData = receivedPieces.get(index);
        System.arraycopy(data, 0, pieceData, begin, data.length);
        receivedBlocks.get(index).set(begin / blockSize);

    }

    public boolean isPieceComplete(int index) {
        int blocksInPiece = (int) Math.ceil((double) pieceLength / blockSize);
        return receivedBlocks.getOrDefault(index, new BitSet()).cardinality() == blocksInPiece;
    }

    public byte[] getAssembledPiece(int index) {
        if (isPieceComplete(index)) {
            return receivedPieces.get(index);
        }
        return null;
    }

    // Удаляем собранную часть после записи
    public void removePiece(int index) {
        receivedPieces.remove(index);
        receivedBlocks.remove(index);
    }

    public boolean validatePieceHash(int index, byte[] pieceData, byte[] pieceHash) {
        byte[] hash = messageDigest.digest(pieceData);

        boolean isValid = Arrays.equals(hash, pieceHash);
        if (isValid) {
            System.out.println("hash is correct: " + index);
        } else {
            System.out.println("hash is not correct: " + index);
        }
        return isValid;
    }
}
