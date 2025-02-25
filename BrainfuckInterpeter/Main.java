import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.lang.reflect.InvocationTargetException;

public class Main {

    public static void main(String[] args) throws ClassNotFoundException, NoSuchMethodException, InstantiationException,
            IllegalArgumentException, IllegalAccessException, InvocationTargetException, IOException {

        InputStream inputStream = null;
        InputStream sourceStream = null;
        OutputStream outputStream = null;

        // if (args.length == 0) {
        // inputStream = System.in;
        // sourceStream = System.in;
        // outputStream = System.out;
        // } else if (args.length == 1) {
        // try {
        // sourceStream = new FileInputStream("source.txt");
        // } catch (FileNotFoundException e) {
        // e.printStackTrace();
        // }
        // inputStream = System.in;
        // outputStream = System.out;
        // } else {
        // try {
        // inputStream = new FileInputStream("input.txt");
        // sourceStream = new FileInputStream("source.txt");
        // outputStream = new FileOutputStream("output.txt");

        // } catch (FileNotFoundException e) {

        // e.printStackTrace();
        // }
        // }

        try {
            inputStream = new FileInputStream("files/input.txt");
            sourceStream = new FileInputStream("files/source.txt");
            outputStream = new FileOutputStream("files/output.txt");

        } catch (FileNotFoundException e) {

            e.printStackTrace();
        }

        BrainfuckInterpreter brainfuckInterpreter = new BrainfuckInterpreter();
        try {
            brainfuckInterpreter.interpret(sourceStream, inputStream, outputStream);
        } catch (IOException e) {
            e.printStackTrace();
        }
        inputStream.close();
        sourceStream.close();
        outputStream.close();
    }
}