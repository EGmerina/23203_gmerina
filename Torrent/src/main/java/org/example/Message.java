package org.example;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.ClosedChannelException;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.SocketChannel;

public class Message {
    private MetaTorrentData metaTorrentData;
    private Selector selector;
    private ByteBuffer buffer;
    private final int BUFFER_SIZE = 1024;

    public Message(Selector selector, MetaTorrentData metaTorrentData) {
        buffer = ByteBuffer.allocate(BUFFER_SIZE);
        this.selector = selector;
        this.metaTorrentData = metaTorrentData;
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

       // byte[] infoHash = metaTorrentData.getInfoHash();
        //buffer.put(infoHash);
        //положить bitfield в метадату????????????????
        buffer.flip();
        client.write(buffer);
    }
}
