package org.example;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.util.Iterator;
import java.util.Set;

public class TorrentServer {
    private int myPort;
    private final int BUFFER_SIZE = 1024;

    public TorrentServer(String port) {
        myPort = Integer.parseInt(port);
    }

    public void start() {
        try {
            Selector selector = Selector.open();
            try {
                ServerSocketChannel socketChannel = ServerSocketChannel.open();
                socketChannel.bind(new InetSocketAddress(myPort));
                socketChannel.configureBlocking(false);
                socketChannel.register(selector, SelectionKey.OP_ACCEPT);

                ByteBuffer buffer = ByteBuffer.allocate(BUFFER_SIZE);
                while (true) {
                    selector.select();
                    Set<SelectionKey> selectedKeys = selector.selectedKeys();
                    Iterator<SelectionKey> iterator = selectedKeys.iterator();

                    while (iterator.hasNext()) {
                        SelectionKey key = iterator.next();
                        if (key.isAcceptable()) {
                            ServerSocketChannel server = (ServerSocketChannel) key.channel();
                            SocketChannel client = server.accept();
                            System.out.println("accept of " + client.getRemoteAddress());
                            client.configureBlocking(false);
                            client.register(selector, SelectionKey.OP_READ);

                        } else if (key.isReadable()) {
                            SocketChannel client = (SocketChannel) key.channel();
                            buffer.clear();
                            // ЗАПУСТИТЬ ОБРАБОТЧИК СООБЩЕНИЯ!
                            // НА ТРЕДПУЛЕ
                            int read = client.read(buffer);
                            if (read == -1) {
                                System.out.println("client was closed (torrect server) " + client.getRemoteAddress());
                                client.close();
                            }else{
                                String message = new String(buffer.array(), 0, read).trim();
                                System.out.println("Received: (torrect server) " + message);
                            }
                        }
                        iterator.remove();
                    }

                }

            } catch (IOException e) {
                System.out.println("can't open SocketChannel in TorrentServer");
                throw new RuntimeException(e);
            }
        } catch (IOException e) {
            System.out.println("can't open selector in TorrentServer");
            throw new RuntimeException(e);
        }

    }
}
