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

    private MetaTorrentData metaTorrentData;
    private Selector selector;
    private MessageHandler messageHandler;

    public TorrentServer(String port, MetaTorrentData metaTorrentData) throws Exception {
        int myPort = Integer.parseInt(port);
        this.metaTorrentData = metaTorrentData;
        selector = Selector.open();
        messageHandler = new MessageHandler(selector, metaTorrentData);

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
                        handleRead(key);
                    }
                    iterator.remove();
                }
            }
        } catch (Exception e) {
            logger.error("problem in handleRead or with select");
            throw new RuntimeException(e);
        }
    }

    //TODO  возможно сдедует поручить обработку messageHandler
    private void handleRead(SelectionKey key) throws Exception {
        SocketChannel client = (SocketChannel) key.channel();

        ByteBuffer bufferForMessageLength = ByteBuffer.allocate(Integer.BYTES);
        int read = client.read(bufferForMessageLength);
        if (read == -1) {
            logger.info("client {} was closed ", client.getRemoteAddress());
            client.close();
            return;
        }
        bufferForMessageLength.flip();
        int messageLength = bufferForMessageLength.getInt();
        ByteBuffer messageBuffer = ByteBuffer.allocate(messageLength);
        read = client.read(messageBuffer);
        if (read == -1) {
            logger.info("client {} was closed ", client.getRemoteAddress());
            client.close();
            return;
        }

        messageBuffer.flip();
        byte byteMessageType = messageBuffer.get();

        MessageTypes messageType;
        try {
            messageType = MessageTypes.values()[byteMessageType];
        } catch (Exception e) {
            logger.error("invalid message type");
            return;
        }
        logger.info("get {} message", messageType);

        switch (messageType) {
            case HANDSHAKE -> {
                byte[] hash = new byte[20];
                messageBuffer.get(hash);

                if (Arrays.equals(hash, metaTorrentData.getInfoHash())) {
                    System.out.println("hashes match ");
                    messageHandler.sendBitField(client);


                } else {
                    System.out.println("hashes don't match => close connection");
                    key.cancel();
                    client.close();
                }
            }
            case KEEPALIVE -> {
                System.out.println("keepalive");
            }
            case REQUEST -> {

                System.out.println("recive request");
                int index = messageBuffer.getInt();
                int begin = messageBuffer.getInt();
                int lenght = messageBuffer.getInt();
                // System.out.println(index + " " + begin + " " + lenght);
                messageHandler.sendPiece(index, begin, lenght, client);

            }
            case HAVE -> {
                System.out.println("have");
            }
            case null, default -> {
                System.out.println("default server message");
            }
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
