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
    private Message message;

    public TorrentServer(String port, MetaTorrentData metaTorrentData) throws IOException {
        myPort = Integer.parseInt(port);
        this.metaTorrentData = metaTorrentData;
        buffer = ByteBuffer.allocate(BUFFER_SIZE);
        selector = Selector.open();
        message = new Message(selector, metaTorrentData);
    }

    //сделать класс хэндлеров отвечающий за каждый key

    public void start() {

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


    }

    private void handleRead(SelectionKey key) throws IOException {
        SocketChannel client = (SocketChannel) key.channel();
        buffer.clear();
        int read = client.read(buffer);
        if (read == -1) {
            System.out.println("client was closed (torrect server) " + client.getRemoteAddress());
            client.close();
            return;
        }

        buffer.rewind();
        byte byteMessageType = buffer.get();
        MessageTypes messageType = MessageTypes.values()[byteMessageType % MessageTypes.values().length]; //TODO try catch invalid message
        System.out.println(messageType);
        switch (messageType) {
            case HANDSHAKE -> {
                byte[] hash = Arrays.copyOfRange(buffer.array(), 1, 21);
                if (Arrays.equals(hash, metaTorrentData.getInfoHash())) {
                    System.out.println("hashes match ");
                    message.sendBitField(client);
                    //может здесь только читать 1 байт????????????????????????????????????
                } else {
                    System.out.println("hashes don't match => close connection");
                    key.cancel();
                    client.close();

                }
            }
            case REQUEST -> {
                System.out.println("recive request");
            }
            case null, default -> {
                System.out.println("default server message");
            }
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
