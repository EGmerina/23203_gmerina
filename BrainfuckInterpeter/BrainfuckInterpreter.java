
import commands.Command;
import commands.MyContext;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.lang.reflect.InvocationTargetException;
import java.util.HashMap;
import java.util.Stack;

public class BrainfuckInterpreter {

    private void parseCommandSymbols(InputStream sourceStream, MyContext context)
            throws IOException, ClassNotFoundException, NoSuchMethodException, InstantiationException,
            IllegalArgumentException, IllegalAccessException, InvocationTargetException {

        char newSymbol;
        Stack<Integer> bordersOfLoops = new Stack<Integer>();

        while ((newSymbol = (char) sourceStream.read()) != -1) {
            if (newSymbol == '[') {
                bordersOfLoops.add(context.getNumberOfCommands());
            }
            if (newSymbol == ']') {
                int loopBegin = bordersOfLoops.pop();
                int loopEnd = context.getNumberOfCommands();
                context.addPointersToLoops(loopBegin, loopEnd);
            }
            Command command = Factory.getInstance().createUnitByName(new String(new char[] { newSymbol }));
            if (command != null) {
                context.addToArrayOfCommands(command);
            }
        }
        if (!bordersOfLoops.isEmpty()) {
            System.err.println("Wrong number of brackets in program!");
        }
    }

    public void interpret(InputStream sourceStream, InputStream inputStream, OutputStream outputStream)
            throws IOException, ClassNotFoundException, NoSuchMethodException,
            InstantiationException, IllegalArgumentException, IllegalAccessException, InvocationTargetException {

        MyContext context = new MyContext(inputStream, outputStream);
        parseCommandSymbols(sourceStream, context);

        while (context.getInstructionPointer() < context.getNumberOfCommands()) {
            context.getCurrentCommand().executeCommand(context);
            context.setInstructionPointer(context.getInstructionPointer() + 1);
        }
    }
}
