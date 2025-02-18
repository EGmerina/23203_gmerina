
import commands.Command;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Hashtable;

public class BrainfuckInterpreter {

    private MyContext context;
    private Hashtable<String, Command> commands;
    // output and input streams

    public BrainfuckInterpreter(InputStream inputStream, OutputStream outputStream) {

    }

    public void interpret(InputStream sourceStream) throws IOException {
        //Factory factory = new Factory();
        while (sourceStream.read() != -1) {

        }
    }
}
