
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class MyContext {

    private byte[] memory;
    private int pointer;

    private InputStream inStream;
    private OutputStream outStream;

    public MyContext() {
        memory = new byte[30000];
        pointer = 0;
    }

    public void incrementCurrentByte() {
        memory[pointer] += 1;
    }

    public void decrementCurrentByte() {
        memory[pointer] -= 1;
    }

    public void incrementPointer() {
        pointer += 1;
    }

    public void decrementPointer() {
        pointer -= 1;
    }

    public void inputCurrentByte() {
        try {
            memory[pointer] = (byte) inStream.read();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void outputCurrentByte() {
        try {
            outStream.write((byte) memory[pointer]);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void loopEnd() {
        if(memory[pointer]!=0){
            
        }
    }

    public void loopBegin() {

    }

    // output stream
}
