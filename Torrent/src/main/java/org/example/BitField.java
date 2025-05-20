package org.example;

public class BitField {
    private byte[] bits;
    private int pieceCount;
    private final int BITS_IN_BYTE = 8;

    public BitField(MetaTorrentData metaTorrentData) {
        this.pieceCount = metaTorrentData.getPiecesNumber();
        bits = new byte[Math.ceilDiv(pieceCount, BITS_IN_BYTE)];
        fillBitField(metaTorrentData);
    }

    private void fillBitField(MetaTorrentData metaTorrentData) {

    }

    public void setPiece(int pieceIndex) {
        int bitPos = BITS_IN_BYTE - 1 - (pieceIndex % BITS_IN_BYTE);
        bits[pieceIndex / BITS_IN_BYTE] |= (1 << bitPos);
    }

}
