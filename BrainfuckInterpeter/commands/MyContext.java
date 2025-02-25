package commands;

import java.io.InputStream;
import java.io.OutputStream;
import java.util.HashMap;
import java.util.Vector;

public class MyContext {

    private byte[] memory = new byte[30000];
    private int dataPointer = 0;

    private Vector<Command> arrayOfCommands;
    private int instructionPointer = -1;

    private InputStream inStream;
    private OutputStream outStream;

    private HashMap<Integer, Integer> pointersToLoops;

    public MyContext(InputStream inputStream, OutputStream outputStream) {
        inStream = inputStream;
        outStream = outputStream;
        Vector<Command> arrayOfCommands = new Vector<Command>();
        HashMap<Integer, Integer> pointersToLoops = new HashMap<Integer, Integer>();
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

    public Command getCommand(int index) {
        return arrayOfCommands.get(index);
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
