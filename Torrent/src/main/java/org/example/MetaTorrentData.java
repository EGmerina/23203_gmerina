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

    public MetaTorrentData(String torrentFile) throws IOException {

        infoHashes = new ArrayList<>();

        Bencode bencode = new Bencode();

        File file = new File(torrentFile);
        byte[] data = new byte[(int) file.length()];

        Path pathToTorrentFile = Paths.get(torrentFile);
        data = Files.readAllBytes(pathToTorrentFile);

        Map<String, Object> torrentData = bencode.decode(data, Type.DICTIONARY);

        Map<String, Object> info = (Map<String, Object>) torrentData.get("info");
        piecesLength = ((Long) info.get("piece length")).intValue();
        sourceFileLength = ((Long) info.get("length")).intValue();
        piecesNumber = sourceFileLength / piecesLength; //TODO тут не округляется в большую сторону
        piecesNumber = (piecesNumber % HASH_LENGTH == 0) ? piecesNumber : piecesNumber + 1;

        String hashes = info.get("pieces").toString();
        int hashesLength = hashes.length();

        for (int i = 0; i < hashesLength; i += HASH_LENGTH) {
            int end = Math.min(i + HASH_LENGTH, hashesLength);
            infoHashes.add(hashes.substring(i, end));
        }
        
    }
}
