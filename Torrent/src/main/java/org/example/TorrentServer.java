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
    private Selector selector;
    private Message message;

    public TorrentServer(String port, MetaTorrentData metaTorrentData) throws Exception {
        myPort = Integer.parseInt(port);
        this.metaTorrentData = metaTorrentData;
        selector = Selector.open();
        //  this.selector = selector;
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
        } catch (Exception e) {
            throw new RuntimeException(e);
        }


    }

    private void handleRead(SelectionKey key) throws Exception {
        SocketChannel client = (SocketChannel) key.channel();
        ByteBuffer buffer = (ByteBuffer) key.attachment();

        buffer.clear();
        int read = client.read(buffer);
        if (read == -1) {
            System.out.println("client was closed (torrect server) " + client.getRemoteAddress());
            client.close();
            return;
        }

        buffer.rewind();
        byte byteMessageType = buffer.get();
        MessageTypes messageType;
        try {
            messageType = MessageTypes.values()[byteMessageType];
        } catch (Exception e) {
            System.out.println("server: invalid message type");
            return;
        }
        System.out.println(messageType);
        switch (messageType) {
            case HANDSHAKE -> {
                byte[] hash = Arrays.copyOfRange(buffer.array(), buffer.position(), buffer.position() + 20);
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
                while (buffer.remaining() >= 12 && messageType == MessageTypes.REQUEST) {

                    System.out.println("recive request");
                    int index = buffer.getInt();
                    int begin = buffer.getInt();
                    int lenght = buffer.getInt();
                    // System.out.println(index + " " + begin + " " + lenght);
                    message.sendPiece(index, begin, lenght, client);

                    if (!buffer.hasRemaining()) {
                        break;
                    }

                    int initialPosition = buffer.position();
                    byteMessageType = buffer.get();
                    messageType = MessageTypes.values()[byteMessageType % MessageTypes.values().length];

                    if (buffer.remaining() < 12 && messageType == MessageTypes.REQUEST) {
                        buffer.position(initialPosition);
                        buffer.compact(); // ожидаем больше данных
                        return;
                    }
                }

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
        ByteBuffer clientBuffer = ByteBuffer.allocate(BUFFER_SIZE);
        client.register(selector, SelectionKey.OP_READ, clientBuffer);
    }
}
