package org.example;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.*;
import java.util.*;

public class TorrentClient {
    private static final Logger logger = LogManager.getLogger(TorrentClient.class);

    private Selector selector;
    private MessageHandler messageHandler;

    public TorrentClient(String[] peers, MetaTorrentData metaTorrentData) throws Exception {
        selector = Selector.open();
        messageHandler = new MessageHandler(metaTorrentData);

        for (String peer : peers) {
            String[] ipAndPort = peer.split(":");
            SocketAddress addr = new InetSocketAddress(ipAndPort[0], Integer.parseInt(ipAndPort[1]));
            SocketChannel newSocketChannel = SocketChannel.open();
            newSocketChannel.configureBlocking(false);
            newSocketChannel.connect(addr);
            newSocketChannel.register(selector, SelectionKey.OP_CONNECT, addr);
        }
    }

    public void start() {
        try {
            while (true) {
                selector.select();
                Set<SelectionKey> selectedKeys = selector.selectedKeys();
                Iterator<SelectionKey> iterator = selectedKeys.iterator();

                while (iterator.hasNext()) {
                    SelectionKey key = iterator.next();

                    if (!key.isValid()) {
                        iterator.remove();
                        continue;
                    }

                    if (key.isConnectable()) {
                        try {
                            handleConnect(key);

                        } catch (Exception e) {
                            tryToConnectAgain(key);
                        }

                    } else if (key.isReadable()) {
                        messageHandler.processMessage(key);
                    }
                    iterator.remove();

                }

            }
        } catch (Exception e) {
            logger.error("problem in handleRead or with select");
            throw new RuntimeException(e);
        }

    }


    private void tryToConnectAgain(SelectionKey key) {
        try {
            SocketChannel newSocketChannel = SocketChannel.open();
            newSocketChannel.configureBlocking(false);
            newSocketChannel.connect((SocketAddress) key.attachment());
            newSocketChannel.register(selector, SelectionKey.OP_CONNECT, (SocketAddress) key.attachment());
        } catch (Exception e) {
            logger.error("can't connect again");
            throw new RuntimeException(e);
        }
    }

    private void handleConnect(SelectionKey key) throws IOException {
        SocketChannel client = (SocketChannel) key.channel();
        if (client.finishConnect()) {
            client.register(selector, SelectionKey.OP_READ);
            messageHandler.sendHandshake(client);
        } else {
            logger.info("wait connection....");
        }

    }

}



