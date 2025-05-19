package org.example;

import com.turn.ttorrent.common.Torrent;

import java.io.File;
import java.io.IOException;
import java.security.NoSuchAlgorithmException;

public class MetaTorrentData {
    private String infoHash;
    private int peacesNumber;
    private int peacesLength;

    public MetaTorrentData(String torrentFile) {

        try {
            Torrent torrent = Torrent.load(new File(torrentFile));
            infoHash = torrent.getHexInfoHash();

        } catch (IOException | NoSuchAlgorithmException e) {
            System.out.println("Problems with parsing torrent file");
            throw new RuntimeException(e);
        }
    }
}
