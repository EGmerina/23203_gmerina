package org.example;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;

public class TorrentServer {
    private int myPort;

    public TorrentServer(String port) {
        myPort = Integer.parseInt(port);
    }

    public void start() {
        try {
            Selector selector = Selector.open();
            try {
                ServerSocketChannel socketChannel = ServerSocketChannel.open();
                socketChannel.bind(new InetSocketAddress(myPort));
                socketChannel.configureBlocking(false);
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
