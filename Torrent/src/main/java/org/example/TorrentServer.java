package org.example;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

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
    private static final Logger logger = LogManager.getLogger(TorrentServer.class);

    private final Selector selector;
    private final MessageHandler messageHandler;

    public TorrentServer(String port, MetaTorrentData metaTorrentData) throws Exception {
        int myPort = Integer.parseInt(port);
        selector = Selector.open();
        messageHandler = new MessageHandler(metaTorrentData);

        ServerSocketChannel socketChannel = ServerSocketChannel.open();
        socketChannel.bind(new InetSocketAddress(myPort));
        socketChannel.configureBlocking(false);
        socketChannel.register(selector, SelectionKey.OP_ACCEPT);

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

                    if (key.isAcceptable()) {
                        handleAccept(key);

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


    private void handleAccept(SelectionKey key) {
        ServerSocketChannel server = (ServerSocketChannel) key.channel();
        try {
            SocketChannel client = server.accept();
            logger.info("accept client {}", client.getRemoteAddress());
            client.configureBlocking(false);
            client.register(selector, SelectionKey.OP_READ);
        } catch (Exception e) {
            logger.error("can't accept client");
            throw new RuntimeException(e);
        }
    }
}
