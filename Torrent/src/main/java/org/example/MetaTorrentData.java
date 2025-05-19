package org.example;

import com.dampcake.bencode.Bencode;
import com.dampcake.bencode.Type;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Map;

public class MetaTorrentData {
    private String infoHash;
    private int peacesNumber;
    private int peacesLength;

    public MetaTorrentData(String torrentFile) throws IOException {
        Bencode bencode = new Bencode();

        File file = new File(torrentFile);
        byte[] data = new byte[(int) file.length()];

        Path pathToTorrentFile = Paths.get(torrentFile);
        data = Files.readAllBytes(pathToTorrentFile);

        Map<String, Object> torrentData = bencode.decode(data, Type.DICTIONARY);

        System.out.println(torrentData.get("date"));
    }
}
