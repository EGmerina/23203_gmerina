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
            // return receivedPieces.get(index); //TODO вот это работает
//            byte[] data = receivedPieces.get(index); //TODO!!!!!!!!!! короче по хорошему в моменте выделять буфферы под каждый кусочек!
//            int endIndex = 0;

            // Ищем первый нулевой байт
//            while (endIndex < data.length - 1 && (data[endIndex] != 0 || data[endIndex + 1] != 0)) {
//                endIndex++;
//            }

//            while (endIndex < data.length && data[endIndex] != 0) {
//                endIndex++;
//            }
//
//            // Копируем только до первого нуля
//            return Arrays.copyOf(data, endIndex);

            byte[] data = receivedPieces.get(index);
            return Arrays.copyOf(data, realSizesOfPieces.get(index)); // TODO это тоже не работает хотя хэши совпадают

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
