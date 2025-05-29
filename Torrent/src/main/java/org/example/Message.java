package org.example;

import java.io.IOException;
import java.net.SocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.ClosedChannelException;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.SocketChannel;
import java.util.*;

public class Message {
    private MetaTorrentData metaTorrentData;
    private Selector selector;
    private ByteBuffer buffer;
    private Map<SocketAddress, BitSet> socketBitFields;
    private final int BUFFER_SIZE = 1024;
    private final int BLOCK_SIZE = 1024 * 16;


    public Message(Selector selector, MetaTorrentData metaTorrentData) {
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
        buffer.put((byte) MessageTypes.HANDSHAKE.ordinal());
        buffer.put(infoHash);
        buffer.flip();
        client.write(buffer);
    }

    public void sendBitField(SocketChannel client) throws IOException {
        buffer.clear();
        System.out.println("send bitfield");
        buffer.put((byte) MessageTypes.BITFIELD.ordinal());

        byte[] bitField = metaTorrentData.getBitField().toByteArray();
        int bitFieldLength = bitField.length;
        buffer.putInt(bitFieldLength);
        // System.out.println(bitField);
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
            return;
        }

        System.out.println("send request");

        Queue<ByteBuffer> requestQueue = new LinkedList<>();
        int blocksInPiece = (int) Math.ceil((double) metaTorrentData.getPiecesLength() / BLOCK_SIZE);

        for (int i = 0; i < blocksInPiece; i++) {
            int begin = i * BLOCK_SIZE;
            int length = Math.min(BLOCK_SIZE, (int) metaTorrentData.getPiecesLength() - begin);

            ByteBuffer request = ByteBuffer.allocate(13);
            request.put((byte) MessageTypes.REQUEST.ordinal());
            request.putInt(index);
            request.putInt(begin);
            request.putInt(length);
            requestQueue.add(request);
        }
        key.cancel();
        client.register(selector, SelectionKey.OP_WRITE, requestQueue);
    }

    private int findMissingPiece(BitSet clientBitFiled) {
        BitSet difference = (BitSet) clientBitFiled.clone();
        BitSet myBitFiled = metaTorrentData.getBitField();
        difference.andNot(myBitFiled);
        int index = difference.nextSetBit(0);
        return index; // -1 если не нашел
    }
}
