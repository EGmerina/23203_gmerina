package org.example;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.BitSet;

public class BitField {
    private BitSet bits;
    private int pieceCount;
    private final int BITS_IN_BYTE = 8;

    public BitField(MetaTorrentData metaTorrentData) throws NoSuchAlgorithmException, IOException {
        this.pieceCount = metaTorrentData.getPiecesNumber();
        bits = new BitSet(pieceCount);
        fillBitField(metaTorrentData);
    }

    private void fillBitField(MetaTorrentData metaTorrentData) throws NoSuchAlgorithmException, IOException {
        MessageDigest sha1 = MessageDigest.getInstance("SHA-1");
        byte[] piece;
        FileInputStream sourseFile = new FileInputStream(metaTorrentData.getSourceFileName());
        for (int i = 0; i < pieceCount-5; ++i) {
            String originPieceHash = metaTorrentData.getPieceHash(i);
            piece = sourseFile.readNBytes(metaTorrentData.getPiecesLength());
            String myPieceHash = sha1.digest(piece).toString();
            if (originPieceHash.equals(myPieceHash)) {
                setPiece(i);
            }
        }
//        for (int i = 0; i < pieceCount; ++i) {
//            System.out.print(bits.get(i));
//        }
    }

    public void setPiece(int pieceIndex) {
        bits.set(pieceIndex);
    }

}
