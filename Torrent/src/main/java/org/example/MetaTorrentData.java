package org.example;

import com.dampcake.bencode.Bencode;
import com.dampcake.bencode.Type;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Map;

public class MetaTorrentData {
    private static final int HASH_LENGTH = 20;
    private ArrayList<String> infoHashes;
    private int piecesNumber;
    private int piecesLength;
    private int sourceFileLength;
    private String sourceFileName;

    public MetaTorrentData(String sourceFile, String torrentFile) throws IOException {

        sourceFileName = sourceFile;

        infoHashes = new ArrayList<>();

        Bencode bencode = new Bencode();

        File file = new File(torrentFile);
        byte[] data;

        Path pathToTorrentFile = Paths.get(torrentFile);
        data = Files.readAllBytes(pathToTorrentFile);

        Map<String, Object> torrentData = bencode.decode(data, Type.DICTIONARY);

        Map<String, Object> info = (Map<String, Object>) torrentData.get("info");
        piecesLength = ((Long) info.get("piece length")).intValue();
        sourceFileLength = ((Long) info.get("length")).intValue();
        piecesNumber = sourceFileLength / piecesLength; //TODO тут не округляется в большую сторону ,  тут вообще не понятно как считается, количество хэшей меньше.
        piecesNumber = (sourceFileLength % piecesLength == 0) ? piecesNumber : piecesNumber + 1;

        String hashes = (String) info.get("pieces");
        // byte[] byteHashes = hashes.getBytes();
        //String hexHashes = bytesToHex(byteHashes);
        int hashesLength = hashes.length();
//        int hashesLengt = hashes.length();
//        int hashesLeng = hexHashes.length();

        for (int i = 0; i < hashesLength; i += HASH_LENGTH) {
            int end = Math.min(i + HASH_LENGTH, hashesLength);
            infoHashes.add(hashes.substring(i, end));
        }

    }

//    public static String bytesToHex(byte[] bytes) {
//        StringBuilder sb = new StringBuilder();
//        for (byte b : bytes) {
//            sb.append(String.format("%02x", b & 0xFF));
//        }
//        return sb.toString();
//    }

    public String getSourceFileName() {
        return sourceFileName;
    }

    public String getPieceHash(int pieceIndex) {
        return infoHashes.get(pieceIndex);
    }

    public int getPiecesNumber() {
        return piecesNumber;
    }

    public int getPiecesLength() {
        return piecesLength;
    }

    public int getSourceFileLength() {
        return sourceFileLength;
    }
}
