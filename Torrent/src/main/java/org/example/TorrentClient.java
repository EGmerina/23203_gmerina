package org.example;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.util.ArrayList;

public class TorrentClient {

    ArrayList<Peer> otherPeers;

    public TorrentClient(String[] peers) {
        otherPeers = new ArrayList<>();
        for (String peer : peers) {
            String[] ipAndPort = peer.split(":");
            Peer newPeer = new Peer(ipAndPort[0], Integer.parseInt(ipAndPort[1]));
            otherPeers.add(newPeer);
        }

    }

    public void start() {
    }

}
