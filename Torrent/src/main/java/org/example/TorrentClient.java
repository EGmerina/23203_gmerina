package org.example;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.*;
import java.util.*;

public class TorrentClient {

    private final int BUFFER_SIZE = 1024;
    private ByteBuffer buffer;
    // private ArrayList<SocketChannel> peersSockets;
    private Selector selector;
    private MetaTorrentData metaTorrentData;
    private Message message;

    public TorrentClient(String[] peers, MetaTorrentData metaTorrentData) throws IOException {
        this.metaTorrentData = metaTorrentData;
        buffer = ByteBuffer.allocate(BUFFER_SIZE);
        // peersSockets = new ArrayList<>();

        selector = Selector.open();
        message = new Message(selector, metaTorrentData);
        for (String peer : peers) {
            String[] ipAndPort = peer.split(":");
            SocketAddress addr = new InetSocketAddress(ipAndPort[0], Integer.parseInt(ipAndPort[1]));
            SocketChannel newSocketChannel = SocketChannel.open();
            newSocketChannel.configureBlocking(false);
            newSocketChannel.connect(addr);
            newSocketChannel.register(selector, SelectionKey.OP_CONNECT, addr);
            // peersSockets.add(newSocketChannel);
        }
    }

    public void start() throws IOException {

        while (true) {
            selector.select();
            Set<SelectionKey> selectedKeys = selector.selectedKeys();
            Iterator<SelectionKey> iterator = selectedKeys.iterator();

            while (iterator.hasNext()) {
                SelectionKey key = iterator.next();

                if (!key.isValid()) {
                    iterator.remove();
                    continue; // пропускаем невалидные ключи
                }

                if (key.isConnectable()) {
                    try {
                        handleConnect(key);

                    } catch (Exception e) {
                        tryToConnectAgain(key);
                    }

                } else if (key.isReadable()) {
                    handleRead(key);
                } else if (key.isWritable()) {
                    handleWrite(key);
                }
                iterator.remove();

            }

        }

    }

    private void handleWrite(SelectionKey key) {
        SocketChannel client = (SocketChannel) key.channel();
        Queue<ByteBuffer> queue = (Queue<ByteBuffer>) key.attachment();

        ByteBuffer newRequest = queue.peek();
        if (newRequest != null) {
            try {
                newRequest.flip();
                client.write(newRequest);

                if (!newRequest.hasRemaining()) {
                    newRequest = null;
                    queue.poll();

                }
            } catch (IOException e) {
                System.err.println("Ошибка записи: " + e.getMessage());
                key.cancel();
            }
        }

        if (queue.isEmpty()) {
            System.out.println("request was sent");
            key.interestOps(SelectionKey.OP_READ);
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
            case BITFIELD -> {
                message.recieveBitField(client, buffer);
                message.sendRequest(client, key);
            }
            case null, default -> {
                System.out.println("default client message");
            }
        }

    }

    private void tryToConnectAgain(SelectionKey key) throws IOException {
        SocketChannel newSocketChannel = SocketChannel.open();
        newSocketChannel.configureBlocking(false);
        newSocketChannel.connect((SocketAddress) key.attachment());
        newSocketChannel.register(selector, SelectionKey.OP_CONNECT, (SocketAddress) key.attachment());
    }

    private void handleConnect(SelectionKey key) throws IOException {

        SocketChannel client = (SocketChannel) key.channel();
        if (client.finishConnect()) {
            message.sendHandshake(client);
        } else {
            System.out.println("wait connection....");
        }

    }

}



