package src;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import src.myexceptions.InterpreterException;
import  org.apache.logging.log4j.Logger;
import  org.apache.logging.log4j.LogManager;

public class Main {

    private static final Logger logger = LogManager.getLogger(Main.class);

    public static void main(String[] args) throws InterpreterException, IOException {

        logger.info("Main starts");
        InputStream inputStream = null;
        InputStream sourceStream = null;
        OutputStream outputStream = null;
        try {
            switch (args.length) {
                case 0 -> {
                    sourceStream = new FileInputStream("files/source.txt");
                    inputStream = System.in;
                    outputStream = System.out;
                }
                case 1 -> {
                    sourceStream = new FileInputStream(args[0]);
                    inputStream = System.in;
                    outputStream = System.out;

                }
                case 2 -> {
                    sourceStream = new FileInputStream(args[0]);
                    inputStream = new FileInputStream(args[1]);
                    outputStream = System.out;
                }
                case 3 -> {
                    sourceStream = new FileInputStream(args[0]);
                    inputStream = new FileInputStream(args[1]);
                    outputStream = new FileOutputStream(args[2]);
                }
                default -> {
                    inputStream = new FileInputStream("files/input.txt");
                    sourceStream = new FileInputStream("files/source.txt");
                    outputStream = new FileOutputStream("files/output.txt");
                }

            }
            logger.info("Streams initialised successfully");
            BrainfuckInterpreter brainfuckInterpreter = new BrainfuckInterpreter();
            brainfuckInterpreter.interpret(sourceStream, inputStream, outputStream);

        } finally {
            inputStream.close();
            sourceStream.close();
            outputStream.close();
            logger.info("Main finished");
        }
    }
}