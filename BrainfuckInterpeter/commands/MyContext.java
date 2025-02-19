package commands;

import java.io.InputStream;
import java.io.OutputStream;

public class MyContext {

    private byte[] memory;
    private int pointer;

    private InputStream srcStream;
    private InputStream inStream;
    private OutputStream outStream;

    public MyContext(InputStream sourceStream, InputStream inputStream, OutputStream outputStream) {
        memory = new byte[30000];
        pointer = 0;
        srcStream = sourceStream;
        inStream = inputStream;
        outStream = outputStream;
    }

    public void setCurrentByte(byte value) {
        memory[pointer] = value;
    }

    public byte getCurrentByte() {
        return memory[pointer];
    }

    public void setPointer(int value) {
        pointer = value;
    }

    public int getPointer() {
        return pointer;
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
