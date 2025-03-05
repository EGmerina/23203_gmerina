package commands;

/**
 * Decrement the data pointer by one (to point to the next cell to the left).
 */
public class Previous implements Command {
    @Override
    public void executeCommand(MyContext context) {
        context.setDataPointer(context.getDataPointer() - 1);
    }
}