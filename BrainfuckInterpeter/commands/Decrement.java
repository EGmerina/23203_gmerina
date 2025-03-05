package commands;

/**
 * Decrement the byte at the data pointer by one.
 */
public class Decrement implements Command {
    @Override
    public void executeCommand(MyContext context) {
        context.setCurrentByte((byte) (context.getCurrentByte() - 1));
    }
}
