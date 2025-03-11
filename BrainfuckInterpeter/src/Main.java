package src;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import src.myexceptions.InterpreterException;

public class Main {

    public static void main(String[] args) throws InterpreterException, IOException {
        // File file = new File("example.txt");
        // if (file.createNewFile()) {
        // System.out.println("Файл создан");
        // } else {
        // System.out.println("Файл уже существует");
        // }
        InputStream inputStream = null;
        InputStream sourceStream = null;
        OutputStream outputStream = null;
        try {
            switch (args.length) {
                case 0 -> {
                    sourceStream = new FileInputStream("src/files/source.txt");
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
                    inputStream = new FileInputStream("src/files/input.txt");
                    sourceStream = new FileInputStream("src/files/source.txt");
                    outputStream = new FileOutputStream("src/files/output.txt");
                }

            }

            BrainfuckInterpreter brainfuckInterpreter = new BrainfuckInterpreter();
            brainfuckInterpreter.interpret(sourceStream, inputStream, outputStream);

        } finally {
            inputStream.close();
            sourceStream.close();
            outputStream.close();
        }
    }
}