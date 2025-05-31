package org.example;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.*;
import java.util.*;

public class TorrentClient {
    private final int BUFFER_SIZE = 1024 * 16 + 13;
    private ByteBuffer buffer;
    private FileWriter fileWriter;
    private Selector selector;
    private MetaTorrentData metaTorrentData;
    private Message message;
    private PieceAssembler pieceAssembler;

    public TorrentClient(String[] peers, MetaTorrentData metaTorrentData) throws Exception {
        this.metaTorrentData = metaTorrentData;
        fileWriter = new FileWriter(metaTorrentData.getSourceFileName(), metaTorrentData.getSourceFileLength());
        buffer = ByteBuffer.allocate(BUFFER_SIZE);
        pieceAssembler = new PieceAssembler((int) metaTorrentData.getPiecesLength());

        selector = Selector.open();
        // this.selector = selector;
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
        int initialPosition = buffer.position();
        byte byteMessageType = buffer.get();
        MessageTypes messageType;
        try {
            messageType = MessageTypes.values()[byteMessageType];
        } catch (Exception e) {
            System.out.println("client: invalid message type");
            return;
        }
        System.out.println(messageType);
        switch (messageType) {
            case BITFIELD -> {
                message.recieveBitField(client, buffer);
                message.sendRequest(client, key);
            }
            case PIECE -> {
                while (buffer.remaining() >= 12 && messageType == MessageTypes.PIECE) {
                    System.out.println("recieve piece");

                    int index = buffer.getInt();
                    int begin = buffer.getInt();
                    int length = buffer.getInt();

                    System.out.println(index + " " + begin + " " + length);
                    if (buffer.remaining() < length) {
                        buffer.position(initialPosition);
                        buffer.compact();
//                        read = client.read(buffer);
//                        if (read == -1 || buffer.remaining() < length) {
//                            break;
//                        }
                    }

                    byte[] data = new byte[length];
                    buffer.get(data);

                    System.out.println(data);

                    pieceAssembler.addBlock(index, begin, data);

                    if (pieceAssembler.isPieceComplete(index)) {
                        byte[] fullPiece = pieceAssembler.getAssembledPiece(index);
                        if (fullPiece != null && pieceAssembler.validatePieceHash(index, fullPiece, metaTorrentData.getPieceHash(index))) {
                            fileWriter.writePiece(index, fullPiece);
                            metaTorrentData.setBitToBitField(index);
                            message.sendHave(client, index);
                            //  message.sendRequest(client, key);
                        } else {
                            message.sendRequest(client, key);
                        }

                        pieceAssembler.removePiece(index); // очищаем память
                    }

                    if (!buffer.hasRemaining()) {
                        break;
                    }

                    initialPosition = buffer.position();
                    byteMessageType = buffer.get();

                    try {
                        messageType = MessageTypes.values()[byteMessageType];
                    } catch (Exception e) {
                        System.out.println("client:piece: invalid message type");
                        buffer.clear();
                        buffer.rewind();
                        return;
                    }
                    if (buffer.remaining() < 12 && messageType == MessageTypes.PIECE) {
                        buffer.position(initialPosition);
                        buffer.compact();
                        return;
                    }
                }
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



