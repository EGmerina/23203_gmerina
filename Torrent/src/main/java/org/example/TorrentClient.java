package org.example;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.util.ArrayList;

public class TorrentClient {

    ArrayList<SocketChannel> peersSockets;

    public TorrentClient(String[] peers) throws IOException {
        peersSockets = new ArrayList<>();
        for (String peer : peers) {
            String[] ipAndPort = peer.split(":");
            SocketAddress addr = new InetSocketAddress(ipAndPort[0], Integer.parseInt(ipAndPort[1]));
            SocketChannel newSocketChannel = SocketChannel.open(addr);
            peersSockets.add(newSocketChannel);
        }

    }

    public void start() {
        for (SocketChannel socketChannel : peersSockets) {
            if(socketChannel.isConnected()){

            }
        }
    }


}
