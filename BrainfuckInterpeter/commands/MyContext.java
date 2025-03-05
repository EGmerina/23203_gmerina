package commands;

import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.HashMap;

/**
 * contains data that nessesary for execution commands.
 */
public class MyContext {

    private final byte[] memory = new byte[30000];
    private int dataPointer = 0;

    private final ArrayList<Command> arrayOfCommands;
    private int instructionPointer = 0;

    private final InputStream inStream;
    private final OutputStream outStream;

    private final HashMap<Integer, Integer> pointersToLoops;

    public MyContext(InputStream inputStream, OutputStream outputStream) {
        inStream = inputStream;
        outStream = outputStream;
        arrayOfCommands = new ArrayList<>();
        pointersToLoops = new HashMap<>();
    }

    public void addToArrayOfCommands(Command command) {
        arrayOfCommands.add(command);
    }

    public void addPointersToLoops(int loopBegin, int loopEnd) {
        pointersToLoops.put(loopBegin, loopEnd);
        pointersToLoops.put(loopEnd, loopBegin);
    }

    public int getBorderOfLoop(int loopEndOrBegin) {
        return pointersToLoops.get(loopEndOrBegin);
    }

    public Command getCurrentCommand() {
        return arrayOfCommands.get(instructionPointer);
    }

    public int getNumberOfCommands() {
        return arrayOfCommands.size();
    }

    public void setCurrentByte(byte value) {
        memory[dataPointer] = value;
    }

    public byte getCurrentByte() {
        return memory[dataPointer];
    }

    public void setDataPointer(int value) {
        dataPointer = value;
    }

    public int getDataPointer() {
        return dataPointer;
    }

    public void setInstructionPointer(int value) {
        instructionPointer = value;
    }

    public int getInstructionPointer() {
        return instructionPointer;
    }

    public InputStream getInputStream() {
        return inStream;
    }

    public OutputStream getOutputStream() {
        return outStream;
    }

}
