
import commands.Command;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.lang.reflect.InvocationTargetException;
import java.util.Stack;
import java.util.Vector;

public class BrainfuckInterpreter {

    private MyContext context;
    private int instructionPointer;
    private Vector<Command> commands;
    private Stack<Integer> beginningsOfCicles;

    // output and input streams

    // private void getCommandsFromSourseStream(InputStream sourseStream) {

    // }

    public BrainfuckInterpreter(InputStream inputStream, OutputStream outputStream) {

    }

    public void interpret(InputStream sourceStream) throws IOException, ClassNotFoundException, NoSuchMethodException,
            InstantiationException, IllegalArgumentException, IllegalAccessException, InvocationTargetException {
        byte command = (byte) sourceStream.read();
        while (command != -1) {

            Command curCommand = Factory.getInstance().createUnitByName(new String(new byte[] { command }));
            curCommand.executeCommand(context);
        }
    }
}
