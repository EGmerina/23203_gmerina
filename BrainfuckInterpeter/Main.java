
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class Main {

    public static void main(String[] args) {

        InputStream inputStream = null;
        InputStream sourceStream = null;
        OutputStream outputStream = null;

        if (args.length == 0) {
            inputStream = System.in;
            sourceStream = System.in;
            outputStream = System.out;
        } else if (args.length == 1) {
            try {
                sourceStream = new FileInputStream("source.txt");
            } catch (FileNotFoundException e) {
                e.printStackTrace();
            }
            inputStream = System.in;
            outputStream = System.out;
        } else {
            try {
                inputStream = new FileInputStream("input.txt");
                sourceStream = new FileInputStream("source.txt");
                outputStream = new FileOutputStream("output.txt");

            } catch (FileNotFoundException e) {

                e.printStackTrace();
            }
        }
        // FileInputStream inputStream = null;
        // FileOutputStream outputStream = null;

        // var cnf =
        // Thread.currentThread().getContextClassLoader().getResourceAsStream("commandsAndNamesOfClasses.config");

        BrainfuckInterpreter brainfuckInterpreter = new BrainfuckInterpreter(inputStream, outputStream);
        try {
            brainfuckInterpreter.interpret(sourceStream);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}