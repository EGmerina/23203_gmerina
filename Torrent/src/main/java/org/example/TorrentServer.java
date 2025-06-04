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

    private int myPort;
    private MetaTorrentData metaTorrentData;
    private Selector selector;
    private Message message;

    public TorrentServer(String port, MetaTorrentData metaTorrentData) throws Exception {
        myPort = Integer.parseInt(port);
        this.metaTorrentData = metaTorrentData;
        selector = Selector.open();
        //  this.selector = selector;
        message = new Message(selector, metaTorrentData);
    }

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
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

    private void handleRead(SelectionKey key) throws Exception {
        SocketChannel client = (SocketChannel) key.channel();

        ByteBuffer bufferForMessageLenght = ByteBuffer.allocate(4);
        int read = client.read(bufferForMessageLenght);
        if (read == -1) {
            System.out.println("client was closed (torrent server) " + client.getRemoteAddress());
            client.close();
            return;
        }
        bufferForMessageLenght.flip();
        int messageLength = bufferForMessageLenght.getInt();
        ByteBuffer messageBuffer = ByteBuffer.allocate(messageLength);
        read = client.read(messageBuffer);
        if (read == -1) {
            System.out.println("client was closed (torrent server) " + client.getRemoteAddress());
            client.close();
            return;
        }
        messageBuffer.flip();

        byte byteMessageType = messageBuffer.get();
        MessageTypes messageType;
        try {
            messageType = MessageTypes.values()[byteMessageType];
        } catch (Exception e) {
            logger.error("server: invalid message type");
            return;
        }
        logger.info("get message: " + messageType);
        switch (messageType) {
            case HANDSHAKE -> {
                // byte[] hash = Arrays.copyOfRange(buffer.array(), buffer.position(), buffer.position() + 20);
                byte[] hash = new byte[20];
                messageBuffer.get(hash);

                if (Arrays.equals(hash, metaTorrentData.getInfoHash())) {
                    System.out.println("hashes match ");
                    message.sendBitField(client);


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
                message.sendPiece(index, begin, lenght, client);

            }
            case HAVE -> {
                System.out.println("have");
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
