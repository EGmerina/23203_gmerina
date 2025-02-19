package commands;

import java.io.InputStream;
import java.io.OutputStream;
import java.util.Stack;
import java.util.Vector;

public class MyContext {

    private byte[] memory = new byte[30000];
    private int dataPointer = 0;

    private InputStream srcStream;
    private InputStream inStream;
    private OutputStream outStream;

    private int instructionPointer = -1;
    private Vector<Command> commands;
    private Stack<Integer> beginningsOfCicles;

    public MyContext(InputStream sourceStream, InputStream inputStream, OutputStream outputStream) {
        srcStream = sourceStream;
        inStream = inputStream;
        outStream = outputStream;
    }

    public void deleteLoop() {
        beginningsOfCicles.pop();
    }

    public Command getCurrentCommand() {
        return commands.get(instructionPointer);
    }

    public int getLastCommandNumber() {
        return commands.size() - 1;
    }

    public void mark() {
        beginningsOfCicles.push(instructionPointer);
    }

    public void reset() {
        instructionPointer = beginningsOfCicles.pop();
    }

    public void add(Command command) {
        commands.add(command);
        instructionPointer += 1;
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

    public InputStream getSourceStream() {
        return srcStream;
    }

    public InputStream getInputStream() {
        return inStream;
    }

    public OutputStream getOutputStream() {
        return outStream;
    }

    // public void incrementCurrentByte() {
    // memory[pointer] += 1;
    // }

    // public void decrementCurrentByte() {
    // memory[pointer] -= 1;
    // }

    // public void incrementPointer() {
    // pointer += 1;
    // }

    // public void decrementPointer() {
    // pointer -= 1;
    // }

    // public void inputCurrentByte() {
    // try {
    // memory[pointer] = (byte) inStream.read();
    // } catch (IOException e) {
    // e.printStackTrace();
    // }
    // }

    // public void outputCurrentByte() {
    // try {
    // outStream.write((byte) memory[pointer]);
    // } catch (IOException e) {
    // e.printStackTrace();
    // }
    // }

    // public void loopEnd() {
    // if (memory[pointer] != 0) {

    // }
    // }

    // public void loopBegin() {

    // }

}
