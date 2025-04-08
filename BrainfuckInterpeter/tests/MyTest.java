package tests;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.charset.StandardCharsets;

import org.apache.logging.log4j.core.util.IOUtils;
import org.junit.After;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;
import src.Main;
import src.BrainfuckInterpreter;
import src.myexceptions.InterpreterException;

import src.commands.Decrement;
import src.commands.Increment;
import src.commands.Input;
import src.commands.MyContext;
import src.commands.Next;
import src.commands.Output;
import src.commands.Previous;

public class MyTest {

    InputStream inStream;
    OutputStream outputStream;
    MyContext context;

    @Before
    public void initStreams() throws FileNotFoundException {
        inStream = new FileInputStream("BrainfuckInterpeter/files/testin.txt");
        outputStream = new FileOutputStream("BrainfuckInterpeter/files/testout.txt");
        context = new MyContext(inStream, outputStream);
    }

    @After
    public void deinitStreams() throws IOException {
        inStream.close();
        outputStream.close();
    }

    @Test
    public void testDecrementCommand() {
        context.setCurrentByte((byte) 3);
        Decrement decrement = new Decrement();
        decrement.executeCommand(context);
        Assert.assertEquals((byte) 2, context.getCurrentByte());
    }

    @Test
    public void testIncrementCommand() {
        context.setCurrentByte((byte) 3);
        Increment increment = new Increment();
        increment.executeCommand(context);
        Assert.assertEquals((byte) 4, context.getCurrentByte());
    }

    @Test
    public void testInputCommand() throws IOException {
        Input input = new Input();
        input.executeCommand(context);
        Assert.assertEquals((byte) 68, context.getCurrentByte());
    }

    @Test
    public void testOutputCommand() throws IOException {
        context.setCurrentByte((byte) 65);
        Output output = new Output();
        output.executeCommand(context);
        InputStream reader = new FileInputStream("BrainfuckInterpeter/files/testout.txt");
        byte myByte = (byte) reader.read();
        Assert.assertEquals((byte) 65, myByte);
        reader.close();
    }

    @Test
    public void testNextCommand() {
        int curDataPointer = context.getDataPointer();
        Next next = new Next();
        next.executeCommand(context);
        Assert.assertEquals(curDataPointer + 1, context.getDataPointer());
    }

    @Test
    public void testPreviousCommand() {
        context.setDataPointer(6);
        Previous previous = new Previous();
        previous.executeCommand(context);
        Assert.assertEquals(5, context.getDataPointer());
    }

    @Test
    public void testHelloWorld() throws FileNotFoundException, IOException, InterpreterException {
        InputStream srcStream = new FileInputStream("BrainfuckInterpeter/files/testhello.txt");
        BrainfuckInterpreter interpreter = new BrainfuckInterpreter();
        interpreter.interpret(srcStream, inStream, outputStream);
        InputStream reader = new FileInputStream("BrainfuckInterpeter/files/testout.txt");
        byte[] buf = new byte[12];
        reader.read(buf, 0, 12);
        String str = new String(buf, StandardCharsets.UTF_8);
        Assert.assertEquals(str, "Hello World!");
        reader.close();
        srcStream.close();

    }

    @Test
    public void testReverse() throws FileNotFoundException, IOException, InterpreterException {
        InputStream srcStream = new FileInputStream("BrainfuckInterpeter/files/testreverse.txt");
        BrainfuckInterpreter interpreter = new BrainfuckInterpreter();
        interpreter.interpret(srcStream, inStream, outputStream);
        InputStream reader = new FileInputStream("BrainfuckInterpeter/files/testout.txt");
        byte[] buf = new byte[4];
        reader.read(buf, 0, 4);
        String str = new String(buf, StandardCharsets.UTF_8);
        Assert.assertEquals(str, "CBAD");
        reader.close();
        srcStream.close();

    }

    @Test
    public void testLoop() {
        InputStream srcStream = new FileInputStream("BrainfuckInterpeter/files/testloop.txt");
        BrainfuckInterpreter interpreter = new BrainfuckInterpreter();
        interpreter.interpret(srcStream, inStream, outputStream);
        InputStream reader = new FileInputStream("BrainfuckInterpeter/files/testout.txt");
        byte[] buf = new byte[1];
        reader.read(buf, 0, 1);
        String str = new String(buf, StandardCharsets.UTF_8);
        Assert.assertEquals(str, "H");
        reader.close();
        srcStream.close();

    }

}
