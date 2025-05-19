package org.example;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;

public class TorrentClient {

    private static int port;
    private static String ip;

    public TorrentClient(String peer) {
        String[] ipAndPort = peer.split(":");
        port = Integer.parseInt(ipAndPort[1]);
        ip = ipAndPort[0];
    }

    private void start() {
        try {
            Selector selector = Selector.open();
            try {
                ServerSocketChannel socketChannel = ServerSocketChannel.open(); //тут сервер, так как мы к нему подключаемся а не наоборот
                socketChannel.bind(new InetSocketAddress(port));
                //socketChannel.configureBlocking(false);
                socketChannel.register(selector, SelectionKey.OP_ACCEPT);
                selector.select();
                //тут должен быть handshake
                //и вообще нужно обрабатывать только одно соединение с известным портом и ip


            } catch (IOException e) {
                System.out.println("can't open SocketChannel in TorrentClient");
                throw new RuntimeException(e);
            }
        } catch (IOException e) {
            System.out.println("can't open selector in TorrentClient");
            throw new RuntimeException(e);
        }

    }

}
