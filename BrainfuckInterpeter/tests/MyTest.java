package tests;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import org.junit.Assert;
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
    @Test
    public void testDecrementCommand() {
        MyContext context = new MyContext(null, null);
        context.setCurrentByte((byte) 3);
        Decrement decrement = new Decrement();
        decrement.executeCommand(context);
        Assert.assertEquals((byte) 2, context.getCurrentByte());
    }

    @Test
    public void testIncrementCommand() {
        MyContext context = new MyContext(null, null);
        context.setCurrentByte((byte) 3);
        Increment increment = new Increment();
        increment.executeCommand(context);
        Assert.assertEquals((byte) 4, context.getCurrentByte());
    }

    @Test
    public void testInputCommand() throws IOException {
        InputStream inputStream = new FileInputStream("in.txt");
        MyContext context = new MyContext(inputStream, null);
        Input input = new Input();
        input.executeCommand(context);
        Assert.assertEquals((byte) 68, context.getCurrentByte());
        inputStream.close();
    }

    @Test
    public void testOutputCommand() throws IOException {
        OutputStream outputStream = new FileOutputStream("out.txt");
        MyContext context = new MyContext(null, outputStream);
        context.setCurrentByte((byte) 65);
        Output output = new Output();
        output.executeCommand(context);
        InputStream reader = new FileInputStream("out.txt");
        byte myByte = (byte) reader.read();
        Assert.assertEquals((byte) 65, myByte);
        outputStream.close();
        reader.close();
    }

    @Test
    public void testNextCommand() {
        MyContext context = new MyContext(null, null);
        int curDatarPointer = context.getDataPointer();
        Next next = new Next();
        next.executeCommand(context);
        Assert.assertEquals(curDatarPointer + 1, context.getDataPointer());
    }

    @Test
    public void testPreviousCommand() {
        MyContext context = new MyContext(null, null);
        context.setDataPointer(6);
        Previous previous = new Previous();
        previous.executeCommand(context);
        Assert.assertEquals(5, context.getDataPointer());
    }

    // @Test
    // public void testHelloWorld() throws FileNotFoundException, IOException {
    //     InputStream srcStream = new FileInputStream("hello.txt");
    //     InputStream inStream = new FileInputStream("in.txt");
    //     OutputStream outputStream = new FileOutputStream("out.txt");
    //     BrainfuckInterpreter interpreter = new BrainfuckInterpreter();

    //     interpreter.interpret(srcStream, inStream, outputStream);

    //     Assert.assertEquals(5, 4);
    //     srcStream.close();
    //     inStream.close();
    //     outputStream.close();
    // }

}
