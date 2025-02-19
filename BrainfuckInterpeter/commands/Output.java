package commands;

import java.io.IOException;

public class Output implements Command {
    @Override
    public void executeCommand(MyContext context) {
        try {
            context.getOutputStream().write(context.getCurrentByte());
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
