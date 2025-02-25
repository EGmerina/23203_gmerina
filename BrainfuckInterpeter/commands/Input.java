package commands;

import java.io.IOException;

public class Input implements Command {
    @Override
    public void executeCommand(MyContext context) {

        try {
            context.setCurrentByte((byte) context.getInputStream().read());
        } catch (IOException e) {
            e.printStackTrace();
        }

    }
}