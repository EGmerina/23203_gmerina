package commands;

import java.io.IOException;

/**
 * Output the byte at the data pointer.
 */
public class Output implements Command {
    @Override
    public void executeCommand(MyContext context) {
        try {
            context.getOutputStream().write(context.getCurrentByte());
        } catch (IOException e) {
            System.err.println("Something wronf with Output command");
        }
    }
}
