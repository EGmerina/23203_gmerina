package org.example;

import com.dampcake.bencode.Bencode;
import com.dampcake.bencode.Type;
import com.turn.ttorrent.bcodec.BDecoder;
import com.turn.ttorrent.bcodec.BEValue;
import com.turn.ttorrent.common.Torrent;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.BitSet;
import java.util.Map;

public class MetaTorrentData {
    private static final int HASH_LENGTH = 20;
    private ArrayList<byte[]> hashes;
    private int piecesNumber;
    private long piecesLength;
    private long sourceFileLength;
    private String sourceFileName;
    private ByteBuffer infoHash;
    private BitSet bitField;

    public MetaTorrentData(String sourceFile, String torrentFile) throws IOException, NoSuchAlgorithmException {

        infoHash = ByteBuffer.allocate(20);
        sourceFileName = sourceFile;

        hashes = new ArrayList<>();

        BDecoder decoder = new BDecoder(new FileInputStream(torrentFile));
        BEValue decoded = decoder.bdecode();
        Map<String, BEValue> root = decoded.getMap();
        BEValue infoValue = root.get("info");

        Map<String, BEValue> infoMap = infoValue.getMap();
        piecesLength = infoMap.get("piece length").getInt();
        sourceFileLength = infoMap.get("length").getInt();
        piecesNumber = (int) (sourceFileLength / piecesLength);
        BEValue pieces = infoMap.get("pieces");
        byte[] byteHashes = pieces.getBytes();

        MessageDigest sha1 = MessageDigest.getInstance("SHA-1"); //TODO тут хэш от хэшей а не от info
        infoHash.put(sha1.digest(byteHashes));
        //infoHash = sha1.digest(byteHashes);

        for (int i = 0; i < byteHashes.length; i += HASH_LENGTH) {
            int end = Math.min(i + HASH_LENGTH, byteHashes.length);
            byte[] newHash = Arrays.copyOfRange(byteHashes, i, end);
            hashes.add(newHash);
        }

        bitField = new BitSet(piecesNumber);
        fillBitField();
    }

    private void fillBitField() throws NoSuchAlgorithmException, IOException {
        MessageDigest sha1 = MessageDigest.getInstance("SHA-1");
        byte[] piece;
        FileInputStream sourseFile = new FileInputStream(sourceFileName);
        for (int i = 0; i < piecesNumber; ++i) {
            byte[] originPieceHash = hashes.get(i);
            piece = sourseFile.readNBytes((int) piecesLength);
            byte[] myPieceHash = sha1.digest(piece);
            if (Arrays.equals(myPieceHash, originPieceHash)) {
                bitField.set(i);
            }
        }
       // System.out.println(bitField);

    }

    public byte[] getInfoHash() {
        return infoHash.array();
    }

    public String getSourceFileName() {
        return sourceFileName;
    }

    public byte[] getPieceHash(int pieceIndex) {
        return hashes.get(pieceIndex);
    }

    public int getPiecesNumber() {
        return piecesNumber;
    }

    public long getPiecesLength() {
        return piecesLength;
    }

    public long getSourceFileLength() {
        return sourceFileLength;
    }

    public BitSet getBitField() {
        return bitField;
    }

    public void setBitToBitField(int index) {
        bitField.set(index);
    }
}
