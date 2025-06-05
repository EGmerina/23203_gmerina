package org.example;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.IOException;
import java.net.SocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.SocketChannel;
import java.util.*;

public class MessageHandler {
    private static final Logger logger = LogManager.getLogger(MessageHandler.class);

    private FileWriter fileWriter;
    private PieceAssembler pieceAssembler;
    private MetaTorrentData metaTorrentData;
    private ByteBuffer buffer;
    private Map<SocketAddress, BitSet> socketBitFields;
    private final int BUFFER_SIZE = 1024 * 17;
    private final int BLOCK_SIZE = 1024 * 16;
    private PieceManager pieceManager;


    public MessageHandler(MetaTorrentData metaTorrentData) throws Exception {
        fileWriter = new FileWriter(metaTorrentData.getSourceFileName(), metaTorrentData.getSourceFileLength());
        pieceAssembler = new PieceAssembler((int) metaTorrentData.getPiecesLength());
        pieceManager = new PieceManager(metaTorrentData.getSourceFileName(), metaTorrentData.getPiecesLength());
        buffer = ByteBuffer.allocate(BUFFER_SIZE);
        this.metaTorrentData = metaTorrentData;
        socketBitFields = new HashMap<>();
    }

    public void sendHandshake(SocketChannel client) throws IOException {
        buffer.clear();
        System.out.println("send handshake");
        byte[] infoHash = metaTorrentData.getInfoHash();
        buffer.putInt(1 + 20);
        buffer.put((byte) MessageTypes.HANDSHAKE.ordinal());
        buffer.put(infoHash);
        buffer.flip();
        client.write(buffer);
    }

    public void sendBitField(SocketChannel client) throws IOException {
        buffer.clear();
        System.out.println("send bitfield");
        byte[] bitField = metaTorrentData.getBitField().toByteArray();
        int bitFieldLength = bitField.length;

        buffer.putInt(1 + 4 + bitFieldLength);

        buffer.put((byte) MessageTypes.BITFIELD.ordinal());
        buffer.putInt(bitFieldLength);
        buffer.put(bitField);
        buffer.flip();
        client.write(buffer);
    }

    public void recieveBitField(SocketChannel client, ByteBuffer buffer) throws IOException {
        int bitFieldLength = buffer.getInt();
        byte[] byteBitField = Arrays.copyOfRange(buffer.array(), buffer.position(), buffer.position() + bitFieldLength);
        BitSet bitField = BitSet.valueOf(byteBitField);
        socketBitFields.put(client.getLocalAddress(), bitField);
        System.out.println("recieve bitField");
    }

    public void sendRequest(SocketChannel client) throws IOException {

        BitSet bitField = socketBitFields.get(client.getLocalAddress());
        int index = findMissingPiece(bitField);
        if (index < 0) {
            System.out.println("not interested");
            return;
        }
        System.out.println("sending request...");
        int blocksInPiece = (int) Math.ceil((double) metaTorrentData.getPiecesLength() / BLOCK_SIZE);
        for (int i = 0; i < blocksInPiece; i++) {

            int begin = i * BLOCK_SIZE;
            int length = Math.min(BLOCK_SIZE, (int) metaTorrentData.getPiecesLength() - begin);

            ByteBuffer request = ByteBuffer.allocate(4 + 1 + 4 * 3);
            request.putInt(1 + 4 * 3);
            request.put((byte) MessageTypes.REQUEST.ordinal());
            request.putInt(index);
            request.putInt(begin);
            request.putInt(length);
            request.flip();
            client.write(request);
        }

    }

    private int findMissingPiece(BitSet clientBitFiled) {
        BitSet difference = (BitSet) clientBitFiled.clone();
        BitSet myBitFiled = metaTorrentData.getBitField();
        difference.andNot(myBitFiled);
        int index = difference.nextSetBit(0);
        if (index < 0) {
            System.out.println("downloading is finished!");
        }
        return index; // -1 если не нашел
    }

    public void sendPiece(int index, int begin, int length, SocketChannel client) throws Exception {
        buffer.clear(); //это буфер message!!!
        System.out.println("send piece");
        ByteBuffer pieceData = pieceManager.readBlock(index, begin, length);

        int realPieceLength = pieceData.array().length;

        System.out.println(realPieceLength);

        buffer.putInt(1 + 4 * 3 + pieceData.array().length);
        buffer.put((byte) MessageTypes.PIECE.ordinal());
        buffer.putInt(index);
        buffer.putInt(begin);
        buffer.putInt(realPieceLength);
        buffer.put(pieceData);
        buffer.flip();
        client.write(buffer);
    }

    public void sendHave(SocketChannel client, int index) throws IOException {
        buffer.clear();
        System.out.println("send have");
        buffer.putInt(1 + 4);
        buffer.put((byte) MessageTypes.HAVE.ordinal());
        buffer.putInt(index);
        buffer.flip();
        client.write(buffer);
    }

    public void processMessage(SelectionKey key) throws Exception {
        SocketChannel client = (SocketChannel) key.channel();

        ByteBuffer bufferForMessageLength = ByteBuffer.allocate(4);
        int read = client.read(bufferForMessageLength);
        if (read == -1) {
            logger.error("client {} was closed ", client.getRemoteAddress());
            client.close();
            return;
        }
        bufferForMessageLength.flip();
        int messageLength = bufferForMessageLength.getInt();
        ByteBuffer messageBuffer = ByteBuffer.allocate(messageLength);
        read = client.read(messageBuffer);
        if (read == -1) {
            logger.error("client {} was closed ", client.getRemoteAddress());
            client.close();
            return;
        }
        messageBuffer.flip();

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
                recieveBitField(client, messageBuffer);
                sendRequest(client);
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
                        sendHave(client, index);

                    }
                    sendRequest(client);

                    pieceAssembler.removePiece(index); // очищаем память
                }

            }
            case HANDSHAKE -> {
                byte[] hash = new byte[20];
                messageBuffer.get(hash);

                if (Arrays.equals(hash, metaTorrentData.getInfoHash())) {
                    System.out.println("hashes match ");
                    sendBitField(client);


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
                sendPiece(index, begin, lenght, client);

            }
            case HAVE -> {
                System.out.println("have");
            }
        }
    }
}
