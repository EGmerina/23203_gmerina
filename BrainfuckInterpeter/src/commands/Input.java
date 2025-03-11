package src.commands;

import java.io.IOException;

/**
 * Accept one byte of input, storing its value in the byte at the data pointer.
 */
public class Input implements Command {
    @Override
    public void executeCommand(MyContext context) {

        try {
            context.setCurrentByte((byte) context.getInputStream().read());
        } catch (IOException e) {
            System.err.println("Something wrong with Input command");
        }

    }
}