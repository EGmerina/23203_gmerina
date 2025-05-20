package org.example;

import java.io.IOException;

import java.security.NoSuchAlgorithmException;
import java.util.ArrayList;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class MyTorrent {
    private static final int BUFFER_SIZE = 1024;
    private static int myPort;
    private static ArrayList<TorrentClient> clients;
    private static final ExecutorService threadPool = Executors.newFixedThreadPool(10);

    public static void main(String[] args) throws IOException, NoSuchAlgorithmException {
        if (args.length < 4) {
            System.out.println("Usage: java TorrentClient <source_file> <torrent-file> <my_port> <peer1-ip:port> [peer2-ip:port ...]");
            return;
        }
        myPort = Integer.parseInt(args[2]);

        MetaTorrentData metaTorrentData = new MetaTorrentData(args[0], args[1]);
        BitField bitField = new BitField(metaTorrentData);

    }
}
/*
 * сервер должен принять все подключения, принимает handshake
 *принимает bitfield
 *
 * клиенты должны запросить подключения, отправляет handshake
 * отправляет bitfield
 *
 * */
//mvn compile
//mvn exec:java -Dexec.mainClass="org.example.MyTorrent"
//java -jar <имя_файла.jar> <аргументы>
//mvn clean package
//  java -jar target/Torrent-1.0-SNAPSHOT-jar-with-dependencies.jar src/main/resources/peer1/EnglishBook.pdf  src/main/resources/peer1/EnglishBook.pdf.torrent  1 1