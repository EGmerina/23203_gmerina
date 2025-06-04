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

    private FileWriter fileWriter;
    private Selector selector;
    private MetaTorrentData metaTorrentData;
    private MessageHandler message;
    private PieceAssembler pieceAssembler;

    public TorrentClient(String[] peers, MetaTorrentData metaTorrentData) throws Exception {
        this.metaTorrentData = metaTorrentData;
        fileWriter = new FileWriter(metaTorrentData.getSourceFileName(), metaTorrentData.getSourceFileLength());
        pieceAssembler = new PieceAssembler((int) metaTorrentData.getPiecesLength());
        selector = Selector.open();
        message = new MessageHandler(selector, metaTorrentData);

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


    private void handleRead(SelectionKey key) throws IOException {
        SocketChannel client = (SocketChannel) key.channel();

        ByteBuffer bufferForMessageLenght = ByteBuffer.allocate(4);
        int read = client.read(bufferForMessageLenght);
        if (read == -1) {
            System.out.println("client was closed (torrect server) " + client.getRemoteAddress());
            client.close();
            return;
        }
        bufferForMessageLenght.flip();
        int messageLength = bufferForMessageLenght.getInt();
        ByteBuffer messageBuffer = ByteBuffer.allocate(messageLength);
        read = client.read(messageBuffer);
        if (read == -1) {
            System.out.println("client was closed (torrect server) " + client.getRemoteAddress());
            client.close();
            return;
        }
        messageBuffer.flip();

        // int initialPosition = buffer.position();
        byte byteMessageType = messageBuffer.get();
        MessageTypes messageType;
        try {
            messageType = MessageTypes.values()[byteMessageType];
        } catch (Exception e) {
            logger.error("client: invalid message type ");
            return;
        }
        logger.info("get {} message", messageType);
        switch (messageType) {
            case BITFIELD -> {
                message.recieveBitField(client, messageBuffer);
                message.sendRequest(client, key);
            }
            case PIECE -> {
                System.out.println("recieve piece");

                int index = messageBuffer.getInt();
                int begin = messageBuffer.getInt();
                int length = messageBuffer.getInt();

                System.out.println(index + " " + begin + " " + length);

                byte[] data = new byte[length];
                messageBuffer.get(data);

                System.out.println(data);

                pieceAssembler.addBlock(index, begin, data);

                if (pieceAssembler.isPieceComplete(index)) {
                    byte[] fullPiece = pieceAssembler.getAssembledPiece(index);
                    //fileWriter.writePiece(index, fullPiece);//!!!!!!!!!!!!!!!! TODO
                    if (fullPiece != null && pieceAssembler.validatePieceHash(index, fullPiece, metaTorrentData.getPieceHash(index))) {
                        fileWriter.writePiece(index, fullPiece);
                        metaTorrentData.setBitToBitField(index);
                        message.sendHave(client, index);

                    }
                    message.sendRequest(client, key);

                    pieceAssembler.removePiece(index); // очищаем память
                }

            }

            case null, default -> {
                System.out.println("default client message");
            }
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
            message.sendHandshake(client);
        } else {
            logger.info("wait connection....");
        }

    }

}



