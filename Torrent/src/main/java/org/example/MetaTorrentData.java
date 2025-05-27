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
import java.security.NoSuchAlgorithmException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Map;

public class MetaTorrentData {
    private static final int HASH_LENGTH = 20;
    private ArrayList<byte[]> infoHashes;
    private int piecesNumber;
    private long piecesLength;
    private long sourceFileLength;
    private String sourceFileName;

    public MetaTorrentData(String sourceFile, String torrentFile) throws IOException, NoSuchAlgorithmException {

        sourceFileName = sourceFile;

        infoHashes = new ArrayList<>();

        // Bencode bencode = new Bencode();

        byte[] data;
        Path pathToTorrentFile = Paths.get(torrentFile);
        data = Files.readAllBytes(pathToTorrentFile);

//        Map<String, Object> torrentData = bencode.decode(data, Type.DICTIONARY);
//
//        Map<String, Object> info = (Map<String, Object>) torrentData.get("info");
//        piecesLength = ((Long) info.get("piece length")).intValue();
//        sourceFileLength = ((Long) info.get("length")).intValue();
//        piecesNumber = (int) (sourceFileLength / piecesLength); //TODO тут не округляется в большую сторону ,  тут вообще не понятно как считается, количество хэшей меньше.
//        piecesNumber = (sourceFileLength % piecesLength == 0) ? piecesNumber : piecesNumber + 1;

        BDecoder decoder = new BDecoder(new FileInputStream(torrentFile));
        BEValue decoded = decoder.bdecode();
        Map<String, BEValue> root = decoded.getMap();
        BEValue infoValue = root.get("info");
        Map<String, BEValue> infoMap = infoValue.getMap();
        piecesLength = infoMap.get("piece length").getInt();
        sourceFileLength = infoMap.get("length").getInt();
        piecesNumber = (int) (sourceFileLength / piecesLength);
        BEValue hashes = infoMap.get("pieces");
        byte[] byteHashes = hashes.getBytes();

        for (int i = 0; i < byteHashes.length; i += HASH_LENGTH) {
            int end = Math.min(i + HASH_LENGTH, byteHashes.length);
            byte[] newHash = Arrays.copyOfRange(byteHashes, i, end);
            infoHashes.add(newHash);
        }


    }


    public String getSourceFileName() {
        return sourceFileName;
    }

    public byte[] getPieceHash(int pieceIndex) {
        return infoHashes.get(pieceIndex);
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
}
