package org.example;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.SocketChannel;
import java.util.*;

//TODO сделать обработку прерываний, have сообщения
public class MessageHandler {
    private static final Logger logger = LogManager.getLogger(MessageHandler.class);

    private final FileWriter fileWriter;
    private final PieceAssembler pieceAssembler;
    private final MetaTorrentData metaTorrentData;
    private final Map<SocketAddress, BitSet> socketBitFields = new HashMap<>();
    private final PieceReader pieceReader;
    private Selector selector;


    public MessageHandler(MetaTorrentData metaTorrentData, Selector selector) throws Exception {
        fileWriter = new FileWriter(metaTorrentData.getSourceFileName(), metaTorrentData.getSourceFileLength(), metaTorrentData.getPiecesLength());
        pieceAssembler = new PieceAssembler((int) metaTorrentData.getPiecesLength());
        pieceReader = new PieceReader(metaTorrentData.getSourceFileName(), metaTorrentData.getPiecesLength());
        this.metaTorrentData = metaTorrentData;
        this.selector = selector;

    }

    public void sendHandshake(SocketChannel client) throws IOException {
        byte[] infoHash = metaTorrentData.getInfoHash();
        ByteBuffer buffer = ByteBuffer.allocate(4 + 1 + 20);
        buffer.putInt(1 + 20);
        buffer.put((byte) MessageTypes.HANDSHAKE.ordinal());
        buffer.put(infoHash);
        buffer.flip();
        client.write(buffer);
        logger.info("send handshake to {}", client.getRemoteAddress());
    }

    public void sendBitField(SocketChannel client) throws IOException {
        byte[] bitField = metaTorrentData.getBitField().toByteArray();
        int bitFieldLength = bitField.length;

        ByteBuffer buffer = ByteBuffer.allocate(4 + 1 + 4 + bitFieldLength); //lengthMessage + messageType +  bitFieldLength + bitFiled
        buffer.putInt(1 + 4 + bitFieldLength);
        buffer.put((byte) MessageTypes.BITFIELD.ordinal());
        buffer.putInt(bitFieldLength);
        buffer.put(bitField);
        buffer.flip();
        client.write(buffer);
        logger.info("send bitfield");
    }

    public void receiveBitField(SocketChannel client, ByteBuffer buffer) throws IOException {
        int bitFieldLength = buffer.getInt();
        byte[] byteBitField = Arrays.copyOfRange(buffer.array(), buffer.position(), buffer.position() + bitFieldLength);
        BitSet bitField = BitSet.valueOf(byteBitField);
        socketBitFields.put(client.getLocalAddress(), bitField);
        logger.info("receive bitField");
    }

    //TODO сделать одиночные requests
    public void sendRequest(SocketChannel client) throws IOException {

        BitSet bitField = socketBitFields.get(client.getLocalAddress());
        int index = findMissingPiece(bitField);
        if (index < 0) {
            logger.info("downloading is finished! => not interested");
            return;
        }
        int BLOCK_SIZE = 1024 * 16;
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
        logger.info("send request");
    }

    private int findMissingPiece(BitSet clientBitFiled) {
        BitSet difference = (BitSet) clientBitFiled.clone();
        BitSet myBitFiled = metaTorrentData.getBitField();
        difference.andNot(myBitFiled);
        int index = difference.nextSetBit(0);
        return index; // -1 если не нашел
    }

    private void receivePiece(SocketChannel client, ByteBuffer messageBuffer) throws IOException {
        int index = messageBuffer.getInt();
        int begin = messageBuffer.getInt();
        int length = messageBuffer.getInt();

        byte[] data = new byte[length];
        messageBuffer.get(data);

        logger.info("receive piece {} {} {}", index, begin, length);

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

    public void sendPiece(int index, int begin, int length, SocketChannel client) throws Exception {
        ByteBuffer pieceData = pieceReader.readBlock(index, begin, length);

        int realPieceLength = pieceData.array().length; //вот это очень старнно! TODO нафиг тогда в сигнатуре length

        ByteBuffer buffer = ByteBuffer.allocate(4 + 1 + 4 * 3 + pieceData.array().length);
        buffer.putInt(1 + 4 * 3 + pieceData.array().length);
        buffer.put((byte) MessageTypes.PIECE.ordinal());
        buffer.putInt(index);
        buffer.putInt(begin);
        buffer.putInt(realPieceLength);
        buffer.put(pieceData);
        buffer.flip();
        client.write(buffer);
        logger.info("send piece with {} length", realPieceLength);
    }

    public void sendHave(SocketChannel client, int index) throws IOException {
        ByteBuffer buffer = ByteBuffer.allocate(4 + 1 + 4);
        buffer.putInt(1 + 4);
        buffer.put((byte) MessageTypes.HAVE.ordinal());
        buffer.putInt(index);
        buffer.flip();
        client.write(buffer);
        logger.info("send have {}th piece", index);
    }

    public void processMessage(SelectionKey key) throws Exception {
        SocketChannel client = (SocketChannel) key.channel();

        ByteBuffer bufferForMessageLength = ByteBuffer.allocate(Integer.BYTES);
        int read;
        try {
            read = client.read(bufferForMessageLength);
        } catch (IOException e) {
            closeClient(key, client);
            return;
        }
        if (read == -1) {
            closeClient(key, client);
            return;
        }
        bufferForMessageLength.flip();
        int messageLength = bufferForMessageLength.getInt();
        ByteBuffer messageBuffer = ByteBuffer.allocate(messageLength);
        read = client.read(messageBuffer);
        if (read == -1) {
            closeClient(key, client);
            return;
        }
        while (read != messageLength) {
            logger.error("can be read only {} / {} bytes. waiting...", read, messageLength);
            Thread.sleep(100);
            read += client.read(messageBuffer);
        }
        messageBuffer.flip();

        byte byteMessageType = messageBuffer.get();
        MessageTypes messageType;
        try {
            messageType = MessageTypes.values()[byteMessageType];
        } catch (Exception e) {
            logger.error("invalid message type ");
            return;
        }
        logger.info("get {} message", messageType);

        switch (messageType) {
            case BITFIELD -> {
                receiveBitField(client, messageBuffer);
                sendRequest(client);
            }
            case PIECE -> {
                receivePiece(client, messageBuffer);
            }
            case HANDSHAKE -> {
                byte[] hash = new byte[20];
                messageBuffer.get(hash);

                if (Arrays.equals(hash, metaTorrentData.getInfoHash())) {
                    logger.info("hashes in handshake match");
                    sendBitField(client);

                } else {
                    logger.info("hashes in handshake don't match => close connection");
                    key.cancel();
                    client.close();
                }
            }
            case REQUEST -> {

                logger.info("receive request");
                int index = messageBuffer.getInt();
                int begin = messageBuffer.getInt();
                int length = messageBuffer.getInt();
                sendPiece(index, begin, length, client);

            }
            case HAVE -> {
                logger.info("receive have");
            }
            case CANCEL -> {
                logger.info("receive cancel");
            }
            case KEEPALIVE -> {
                logger.info("receive keepalive");
            }
        }
    }

    private void closeClient(SelectionKey key, SocketChannel client) throws IOException {
        logger.error("client {} was closed ", client.getRemoteAddress());
        SocketAddress addr = (SocketAddress) key.attachment();
        key.cancel();
        client.close();
        SocketChannel newSocketChannel = SocketChannel.open();
        newSocketChannel.configureBlocking(false);
        try {
            newSocketChannel.connect(addr);
        } catch (Exception e) {
            logger.error("waiting for connection");
        }
        newSocketChannel.register(selector, SelectionKey.OP_CONNECT, addr);
    }


}
