package org.example;

import java.nio.channels.SocketChannel;

public enum MessageTypes {
    KEEPALIVE, HANDSHAKE, HAVE, BITFIELD, REQUEST, PIECE, CANCEL;

}
