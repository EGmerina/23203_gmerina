package org.example;

import com.dampcake.bencode.Bencode;
import com.dampcake.bencode.Type;

import javax.sound.sampled.Port;
import java.io.File;
import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class MyTorrent {
    private static final int BUFFER_SIZE = 1024;
    private static int myPort;
    private static ArrayList<TorrentClient> clients;
    private static final ExecutorService threadPool = Executors.newFixedThreadPool(10);

    public static void main(String[] args) throws IOException {
        if (args.length < 3) {
            System.out.println("Usage: java TorrentClient <torrent-file> <my_port> <peer1-ip:port> [peer2-ip:port ...]");
            return;
        }
        myPort = Integer.parseInt(args[1]);

       // MetaTorrentData metaTorrentData = new MetaTorrentData(args[0]);

        Bencode bencode = new Bencode();

        File file = new File(args[0]);
        byte[] data = new byte[(int) file.length()];

        Path pathToTorrentFile = Paths.get(args[0]);
        data = Files.readAllBytes(pathToTorrentFile);

        Map<String, Object> torrentData = bencode.decode(data, Type.DICTIONARY);

        //System.out.println(torrentData);
        System.out.println(torrentData.get("creation date"));

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
//  java -jar target/Torrent-1.0-SNAPSHOT-jar-with-dependencies.jar src/main/resources/peer1/EnglishBook.pdf.torrent  1 1