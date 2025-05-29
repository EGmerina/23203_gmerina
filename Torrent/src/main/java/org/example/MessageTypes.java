package org.example;

import java.nio.channels.SocketChannel;

public enum MessageTypes {
    KEEPALIVE, HANDSHAKE, INTERESTED, NOT_INTERESTED, HAVE, BITFIELD, REQUEST, PIECE, CANCEL;

}
