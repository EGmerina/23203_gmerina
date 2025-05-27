package org.example;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.util.Arrays;
import java.util.Iterator;
import java.util.Set;

public class TorrentServer {
    private int myPort;
    private final int BUFFER_SIZE = 1024;
    private MetaTorrentData metaTorrentData;
    private ByteBuffer buffer;
    private Selector selector;

    public TorrentServer(String port, MetaTorrentData metaTorrentData) {
        myPort = Integer.parseInt(port);
        this.metaTorrentData = metaTorrentData;
        buffer = ByteBuffer.allocate(BUFFER_SIZE);
    }

    //сделать класс хэндлеров отвечающий за каждый key

    public void start() {
        try {
            selector = Selector.open();
            try {
                ServerSocketChannel socketChannel = ServerSocketChannel.open();
                socketChannel.bind(new InetSocketAddress(myPort));
                socketChannel.configureBlocking(false);
                socketChannel.register(selector, SelectionKey.OP_ACCEPT);

                while (true) {
                    selector.select();
                    Set<SelectionKey> selectedKeys = selector.selectedKeys();
                    Iterator<SelectionKey> iterator = selectedKeys.iterator();

                    while (iterator.hasNext()) {
                        SelectionKey key = iterator.next();
                        if (key.isAcceptable()) {
                            handleAccept(key);

                        } else if (key.isReadable()) {
                            handleRead(key);
                        }
                        iterator.remove();
                    }

                }

            } catch (IOException e) {
                System.out.println("can't open SocketChannel in TorrentServer");
                throw new RuntimeException(e);
            }
        } catch (IOException e) {
            System.out.println("can't open selector in TorrentServer");
            throw new RuntimeException(e);
        }

    }

    private void handleRead(SelectionKey key) throws IOException {
        SocketChannel client = (SocketChannel) key.channel();
        buffer.clear();
        // ЗАПУСТИТЬ ОБРАБОТЧИК СООБЩЕНИЯ! НА ТРЕДПУЛЕ
        int read = client.read(buffer);
        if (read == -1) {
            System.out.println("client was closed (torrect server) " + client.getRemoteAddress());
            client.close();
        } else if (Arrays.equals(buffer.array(), metaTorrentData.getInfoHash())) {
            System.out.println("hashes match ");
        } else {
            System.out.println(metaTorrentData.getInfoHash());
        }
    }

    private void handleAccept(SelectionKey key) throws IOException {
        ServerSocketChannel server = (ServerSocketChannel) key.channel();
        SocketChannel client = server.accept();
        System.out.println("accept of " + client.getRemoteAddress());
        client.configureBlocking(false);
        client.register(selector, SelectionKey.OP_READ);
    }
}
