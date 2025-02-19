
import commands.Command;
import commands.MyContext;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.lang.reflect.InvocationTargetException;

public class BrainfuckInterpreter {

    private MyContext context;
    // private int instructionPointer;
    // private Vector<Command> commands;
    // private Stack<Integer> beginningsOfCicles;

    // output and input streams

    // private void getCommandsFromSourseStream(InputStream sourseStream) {

    // }

    public BrainfuckInterpreter(InputStream sourceStream, InputStream inputStream, OutputStream outputStream) {
        context = new MyContext(sourceStream, inputStream, outputStream);
    }

    public void interpret() throws IOException, ClassNotFoundException, NoSuchMethodException,
            InstantiationException, IllegalArgumentException, IllegalAccessException, InvocationTargetException {
        byte command = (byte) context.getSourceStream().read();
        while (command != -1) {

            Command curCommand = Factory.getInstance().createUnitByName(new String(new byte[] { command }));
            curCommand.executeCommand(context);
        }
    }
}
