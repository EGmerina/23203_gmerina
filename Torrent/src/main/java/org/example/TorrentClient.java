package org.example;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.Set;

public class TorrentClient {

    private ArrayList<SocketChannel> peersSockets;
    private final int BUFFER_SIZE = 1024;
    private Selector selector;
    private ByteBuffer buffer;
    private MetaTorrentData metaTorrentData;

    public TorrentClient(String[] peers, MetaTorrentData metaTorrentData) throws IOException {
        this.metaTorrentData = metaTorrentData;
        buffer = ByteBuffer.allocate(BUFFER_SIZE);
        peersSockets = new ArrayList<>();
        selector = Selector.open();
        for (String peer : peers) {
            String[] ipAndPort = peer.split(":");
            SocketAddress addr = new InetSocketAddress(ipAndPort[0], Integer.parseInt(ipAndPort[1]));
            SocketChannel newSocketChannel = SocketChannel.open();
            newSocketChannel.configureBlocking(false);
            newSocketChannel.connect(addr);
            newSocketChannel.register(selector, SelectionKey.OP_CONNECT, addr);
            peersSockets.add(newSocketChannel);
        }
    }

    public void start() throws IOException {

        while (true) {
            selector.select();
            Set<SelectionKey> selectedKeys = selector.selectedKeys();
            Iterator<SelectionKey> iterator = selectedKeys.iterator();

            while (iterator.hasNext()) {
                SelectionKey key = iterator.next();

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
        //                    SocketChannel client = (SocketChannel) key.channel();
//                    buffer.clear();
//                    String message = "hello from " + client.getRemoteAddress();
//                    byte[] messageBytes = message.getBytes(StandardCharsets.UTF_8);
//                    buffer.put(messageBytes);
//                    buffer.flip();
//                    client.write(buffer);
    }

    private void handleRead(SelectionKey key) throws IOException {
        SocketChannel client = (SocketChannel) key.channel();
        buffer.clear();
        // TODO ЗАПУСТИТЬ ОБРАБОТЧИК СООБЩЕНИЯ! threadpool
        int read = client.read(buffer);
        if (read == -1) {
            System.out.println("client was closed (torrect client) " + client.getRemoteAddress());
            key.cancel();
            client.close();
        } else {
            String message = new String(buffer.array(), 0, read).trim();
            System.out.println("Received: (torrect client) " + message);
        }
    }

    private void tryToConnectAgain(SelectionKey key) throws IOException {
        SocketChannel newSocketChannel = SocketChannel.open();
        newSocketChannel.configureBlocking(false);
        newSocketChannel.connect((SocketAddress) key.attachment());
        newSocketChannel.register(selector, SelectionKey.OP_CONNECT, (SocketAddress) key.attachment());
    }

    private void handleConnect(SelectionKey key) throws IOException {
        //handshake
        buffer.clear();
        SocketChannel client = (SocketChannel) key.channel();
        if (client.finishConnect()) {
            System.out.println("send message");
            client.register(selector, SelectionKey.OP_READ);
            byte[] infoHash = metaTorrentData.getInfoHash();
            buffer.put(infoHash);
            buffer.flip();
            client.write(buffer);
        } else {
            System.out.println("wait connection....");
        }

    }

}



