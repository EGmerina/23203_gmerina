package org.example;

import javax.sound.sampled.Port;
import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.util.ArrayList;
import java.util.Iterator;
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

        MetaTorrentData metaTorrentData = new MetaTorrentData(args[0]);
//        for (String peer : args) {
//            TorrentClient newTorrentClient = new TorrentClient(peer);
//            clients.add(newTorrentClient);
//        }

        startDownloading();
        startSeeding();

        Selector selector = Selector.open();

        ServerSocketChannel serverChannel = ServerSocketChannel.open();
        serverChannel.bind(new InetSocketAddress(myPort));
        serverChannel.configureBlocking(false);
        serverChannel.register(selector, SelectionKey.OP_ACCEPT);

        ByteBuffer buffer = ByteBuffer.allocate(BUFFER_SIZE);

        while (true){
            selector.select();
            Set<SelectionKey> selectedKeys = selector.selectedKeys();
            Iterator<SelectionKey> iter = selectedKeys.iterator();

        }

        threadPool.submit(() -> {
            String message = new String(data).trim();
            System.out.println("Processing in thread " + Thread.currentThread().getName() + ": " + message);

            // Тяжёлая обработка
            try {
                Thread.sleep(1000); // Имитация долгой обработки
                ByteBuffer response = ByteBuffer.wrap(("Echo: " + message).getBytes());
                client.write(response);
            } catch (Exception e) {
                e.printStackTrace();
            }
        });


    }

    private static void startSeeding() {
    }

    private static void startDownloading() {

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
