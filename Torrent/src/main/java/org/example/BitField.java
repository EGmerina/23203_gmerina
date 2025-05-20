package org.example;

import java.io.FileInputStream;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.BitSet;

public class BitField {
    private BitSet bits;
    private int pieceCount;
    private final int BITS_IN_BYTE = 8;

    public BitField(MetaTorrentData metaTorrentData) throws NoSuchAlgorithmException {
        this.pieceCount = metaTorrentData.getPiecesNumber();
        bits = new BitSet(pieceCount);
        fillBitField(metaTorrentData);
    }

    private void fillBitField(MetaTorrentData metaTorrentData) throws NoSuchAlgorithmException {
        MessageDigest sha1 = MessageDigest.getInstance("SHA-1");
        //FileInputStream sourseFile = new FileInputStream("");
        for (int i = 0; i < pieceCount; ++i) {
            String originPieceHash = metaTorrentData.getPieceHash(i);
            //String myPieceHash =
        }
    }

    public void setPiece(int pieceIndex) {
        bits.set(pieceIndex);
    }

}
