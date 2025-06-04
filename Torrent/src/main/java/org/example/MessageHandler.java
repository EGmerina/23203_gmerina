package org.example;

import java.io.IOException;
import java.net.SocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.SocketChannel;
import java.util.*;

public class MessageHandler {
    private MetaTorrentData metaTorrentData;
    private Selector selector;
    private ByteBuffer buffer;
    private Map<SocketAddress, BitSet> socketBitFields;
    private final int BUFFER_SIZE = 1024 * 17;
    private final int BLOCK_SIZE = 1024 * 16;
    private PieceManager pieceManager;


    public MessageHandler(Selector selector, MetaTorrentData metaTorrentData) throws Exception {
        pieceManager = new PieceManager(metaTorrentData.getSourceFileName(), metaTorrentData.getPiecesLength());
        buffer = ByteBuffer.allocate(BUFFER_SIZE);
        this.selector = selector;
        this.metaTorrentData = metaTorrentData;
        socketBitFields = new HashMap<>();
    }

    public void sendHandshake(SocketChannel client) throws IOException {
        buffer.clear();
        System.out.println("send handshake");
        client.register(selector, SelectionKey.OP_READ);
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

    public void sendRequest(SocketChannel client, SelectionKey key) throws IOException {

        BitSet bitField = socketBitFields.get(client.getLocalAddress());
        int index = findMissingPiece(bitField);
        if (index < 0) {
            System.out.println("not interested");
//            buffer.clear();
//            buffer.put((byte) MessageTypes.KEEPALIVE.ordinal());
//            buffer.flip();
//            client.write(buffer);
            return;
        }

        System.out.println("sending request...");

        Queue<ByteBuffer> requestQueue = new LinkedList<>();
        int blocksInPiece = (int) Math.ceil((double) metaTorrentData.getPiecesLength() / BLOCK_SIZE);

        for (int i = 0; i < blocksInPiece; i++) {
            int begin = i * BLOCK_SIZE;
            int length = Math.min(BLOCK_SIZE, (int) metaTorrentData.getPiecesLength() - begin);

            ByteBuffer request = ByteBuffer.allocate(17);
            request.putInt(1 + 4 * 3);
            request.put((byte) MessageTypes.REQUEST.ordinal());
            request.putInt(index);
            request.putInt(begin);
            request.putInt(length);
            request.flip();
            requestQueue.add(request);
        }
        key.attach(requestQueue);
        key.interestOps(SelectionKey.OP_WRITE);
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
}
