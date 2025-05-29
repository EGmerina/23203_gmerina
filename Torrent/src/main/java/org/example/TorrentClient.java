package org.example;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.*;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Iterator;
import java.util.Set;

public class TorrentClient {

    private final int BUFFER_SIZE = 1024;
    private ByteBuffer buffer;
    private ArrayList<SocketChannel> peersSockets;
    private Selector selector;
    private MetaTorrentData metaTorrentData;
    private Message message;

    public TorrentClient(String[] peers, MetaTorrentData metaTorrentData) throws IOException {
        this.metaTorrentData = metaTorrentData;
        buffer = ByteBuffer.allocate(BUFFER_SIZE);
        peersSockets = new ArrayList<>();
        selector = Selector.open();
        message = new Message(selector, metaTorrentData);
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

                }
                iterator.remove();

            }

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
        MessageTypes message = MessageTypes.values()[buffer.get(0) % MessageTypes.values().length];
        switch (message) {
            case BITFIELD -> {}

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



