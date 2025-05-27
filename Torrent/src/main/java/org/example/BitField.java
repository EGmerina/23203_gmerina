package org.example;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Arrays;
import java.util.BitSet;

public class BitField {
    private BitSet bits;
    private int pieceCount;

    public BitField(MetaTorrentData metaTorrentData) throws NoSuchAlgorithmException, IOException {
        this.pieceCount = metaTorrentData.getPiecesNumber();
        bits = new BitSet(pieceCount);
        fillBitField(metaTorrentData);
    }

    private void fillBitField(MetaTorrentData metaTorrentData) throws NoSuchAlgorithmException, IOException {
        MessageDigest sha1 = MessageDigest.getInstance("SHA-1");
        byte[] piece;
        FileInputStream sourseFile = new FileInputStream(metaTorrentData.getSourceFileName());
        for (int i = 0; i < pieceCount; ++i) {
            byte[] originPieceHash = metaTorrentData.getPieceHash(i);
            piece = sourseFile.readNBytes((int)metaTorrentData.getPiecesLength());
            byte[] myPieceHash = sha1.digest(piece);
            if (Arrays.equals(myPieceHash, originPieceHash)) {
                setPiece(i);
            }
        }

    }

    public void setPiece(int pieceIndex) {
        bits.set(pieceIndex);
    }

}
