package src;
import src.commands.Command;
import src.commands.LoopBegin;
import src.commands.LoopEnd;
import src.commands.MyContext;
import src.myexceptions.FactoryException;
import src.myexceptions.InterpreterException;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Stack;

/**
 * Interpret brainfuck code with interpret() function.
 */
public class BrainfuckInterpreter {

    private void parseCommandSymbols(InputStream sourceStream, MyContext context) throws IOException, FactoryException {

        byte newSymbol;
        Stack<Integer> bordersOfLoops = new Stack<>();
        Factory factory = new Factory();
        while ((newSymbol = (byte) sourceStream.read()) != -1) {
            Command command = factory.createUnitByName(new String(new byte[] { newSymbol
            }));
            if (command == null) {
                continue;
            }

            if (command.getClass() == LoopBegin.class) {
                bordersOfLoops.add(context.getNumberOfCommands());
            }
            if (command.getClass() == LoopEnd.class) {
                int loopBegin = bordersOfLoops.pop();
                int loopEnd = context.getNumberOfCommands();
                context.addPointersToLoops(loopBegin, loopEnd);
            }
            context.addToArrayOfCommands(command);
        }
        if (!bordersOfLoops.isEmpty()) {
            System.err.println("Wrong number of brackets in program!");
        }
    }

    /**
     * 
     * @param sourceStream for output result
     * @param inputStream  for input data
     * @param outputStream for output data
     * @throws InterpreterException
     */
    public void interpret(InputStream sourceStream, InputStream inputStream, OutputStream outputStream)
            throws InterpreterException {

        MyContext context = new MyContext(inputStream, outputStream);
        try {
            parseCommandSymbols(sourceStream, context);

        } catch (IOException | FactoryException e) {
            throw new InterpreterException("Interpreter can't parse command symbols");
        }

        while (context.getInstructionPointer() < context.getNumberOfCommands()) {
            context.getCurrentCommand().executeCommand(context);
            context.setInstructionPointer(context.getInstructionPointer() + 1);
        }
    }
}